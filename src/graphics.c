#include "graphics.h"

#include "cglm/cam.h"
#include "cglm/types.h"
#include "shader.h"

#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/vec2.h>
#include <stdio.h>
#include <stb_image/stb_image.h>

#define MAX_QUADS 100
#define MAX_VERTICES MAX_QUADS * 4
#define MAX_INDICES MAX_QUADS * 6

const char* default_shader_vert_source =
  "#version 330 core\n\
  layout(location = 0) in vec2 a_position;\n\
  layout(location = 1) in vec2 a_local_position;\n\
  layout(location = 2) in vec3 a_color;\n\
  layout(location = 3) in vec2 a_uv;\n\
  layout(location = 4) in uint a_type;\n\
  uniform mat4 u_projection;\n\
  uniform mat4 u_view;\n\
  out vec2 local_position;\n\
  out vec3 color;\n\
  out vec2 uv;\n\
  flat out uint type;\n\
  void main() {\n\
    gl_Position = u_projection * u_view * vec4(a_position, 0.0, 1.0);\n\
    local_position = a_local_position;\n\
    color = a_color;\n\
    uv = a_uv;\n\
    type = a_type;\n\
  }";

const char* framebuffer_shader_vert_source =
  "#version 330 core\n\
  layout(location = 0) in vec2 a_position;\n\
  layout(location = 1) in vec2 a_uv;\n\
  out vec2 uv;\n\
  void main() {\n\
    gl_Position = vec4(a_position, 0.0, 1.0);\n\
    uv = a_uv;\n\
  }";

const char* framebuffer_shader_frag_source =
  "#version 330 core\n\
  in vec2 uv;\n\
  out vec4 out_color;\n\
  uniform sampler2D u_texture;\n\
  void main() {\n\
    out_color = texture(u_texture, uv);\n\
  }";

const char* texture_shader_frag_source =
  "#version 330 core\n\
  in vec2 local_position;\n\
  in vec3 color;\n\
  in vec2 uv;\n\
  flat in uint type;\n\
  uniform sampler2D u_texture;\n\
  out vec4 out_color;\n\
  void main() {\n\
    if (type == 0u) {\n\
      out_color = texture(u_texture, uv);\n\
    } else if (type == 1u) {\n\
      float dist = 1.0 - length(local_position);\n\
      if (dist < 0.0) { discard; }\n\
      out_color = vec4(color, 1.0);\n\
    } else {\n\
      out_color = vec4(color, 1.0);\n\
    }\n\
  }";

struct Vertex {
  vec2 position;
  vec2 local_position;
  vec3 color;
  vec2 uv;
  unsigned int type;
};

struct FramebufferVertex {
  vec2 position;
  vec2 uv;
};

struct Framebuffer {
  unsigned int fbo;
  unsigned int vao;
  unsigned int vbo;
  unsigned int shader;
  unsigned int texture;
  struct FramebufferVertex vertices[6];
};

struct GraphicsData {
  GLFWwindow* window;
  unsigned int vao;
  unsigned int vbo;
  unsigned int shader;
  unsigned int quad_count;
  unsigned int texture;
  vec2 texture_size;
  vec2 game_size;
  ivec2 window_size;
  struct Framebuffer framebuffer;
  struct Vertex* vertices;
  mat4 projection;
  mat4 view;
};

static struct GraphicsData data;

static void graphics_flush() {
  shader_use(data.shader);
  glBindVertexArray(data.vao);
  glBufferSubData(
    GL_ARRAY_BUFFER,
    0,
    sizeof(struct Vertex) * data.quad_count * 4,
    data.vertices
  );
  glDrawElements(GL_TRIANGLES, data.quad_count * 6, GL_UNSIGNED_INT, NULL);
  data.quad_count = 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  data.window_size[0] = width;
  data.window_size[1] = height;
}

unsigned int graphics_load_texture(const char* path) {
  int width, height, channels;
  unsigned char* image_data = stbi_load(path, &width, &height, &channels, 0);
  if (image_data == NULL) {
    fprintf(stderr, "Failed to load image\n");
  }
  
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA,
    (unsigned int)width, (unsigned int)height,
    0, GL_RGBA, GL_UNSIGNED_BYTE,
    image_data
  );
  
  stbi_image_free(image_data);

  data.texture_size[0] = (float)width;
  data.texture_size[1] = (float)height;
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  return texture;
}

void graphics_use_texture(unsigned int texture) {
  if (texture != data.texture) {
    shader_use(data.shader);
    data.texture = texture;
    unsigned int u_texture = glGetUniformLocation(data.shader, "u_texture");
    // TODO: why 0 and not texture (i think it's because its GL_TEXTURE0 not the texture id)
    glUniform1i(u_texture, 0);
  }  
}

void graphics_init(const char* title, const vec2 size) {
  int glfwInit_result = glfwInit();
  if (!glfwInit_result) {
    fprintf(stderr, "glfwInit failed\n");
  }
  
  data.vertices = malloc(sizeof(struct Vertex) * MAX_VERTICES);
  
  glm_vec2_copy(size, data.game_size);
  int initial_width = 1000;
  int initial_height = 1000.0f / (size[0] / size[1]);
    
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  data.window = glfwCreateWindow(initial_width, initial_height, title, NULL, NULL);

  if (!data.window) {
    fprintf(stderr, "glfwCreateWindow failed\n");
    glfwTerminate();
  }

  glfwGetFramebufferSize(data.window, &data.window_size[0], &data.window_size[1]);
  
  glfwMakeContextCurrent(data.window);
  glfwSetWindowAspectRatio(data.window, size[0], size[1]);
  glfwSwapInterval(1);
  
  int gladLoadGl_result = gladLoadGL(glfwGetProcAddress);
  if (!gladLoadGl_result) {
    fprintf(stderr, "gladLoadGL failed");
    glfwDestroyWindow(data.window);
    glfwTerminate();
  }

  glfwSetFramebufferSizeCallback(data.window, framebuffer_size_callback);

  // framebuffer
  data.framebuffer.shader = shader_create(
    framebuffer_shader_vert_source,
    framebuffer_shader_frag_source
  );
    
  data.framebuffer.vertices[0] = (struct FramebufferVertex){
    .position = { -1.0f, -1.0f },
    .uv = { 0.0f, 0.0f },
  };
  data.framebuffer.vertices[1] = (struct FramebufferVertex){
    .position = { -1.0f, 1.0f },
    .uv = { 0.0f, 1.0f },
  };
  data.framebuffer.vertices[2] = (struct FramebufferVertex){
    .position = { 1.0f, -1.0f },
    .uv = { 1.0f, 0.0f },
  };
  data.framebuffer.vertices[3] = (struct FramebufferVertex){
    .position = { 1.0f, -1.0f },
    .uv = { 1.0f, 0.0f },
  };
  data.framebuffer.vertices[4] = (struct FramebufferVertex){
    .position = { -1.0f, 1.0f },
    .uv = { 0.0f, 1.0f },
  };
  data.framebuffer.vertices[5] = (struct FramebufferVertex){
    .position = { 1.0f, 1.0f },
    .uv = { 1.0f, 1.0f },
  };
    
  glGenVertexArrays(1, &data.framebuffer.vao);      
  glBindVertexArray(data.framebuffer.vao);
  
  glGenBuffers(1, &data.framebuffer.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, data.framebuffer.vbo);
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(struct FramebufferVertex) * 6,
    data.framebuffer.vertices,
    GL_STATIC_DRAW
  );
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct FramebufferVertex),
    (const void*)offsetof(struct FramebufferVertex, position)
  );
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct FramebufferVertex),
    (const void*)offsetof(struct FramebufferVertex, uv)
  );
  
  glGenFramebuffers(1, &data.framebuffer.fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, data.framebuffer.fbo);
  
  glGenTextures(1, &data.framebuffer.texture);
  glBindTexture(GL_TEXTURE_2D, data.framebuffer.texture);
  glTexImage2D(
    GL_TEXTURE_2D,
    0, GL_RGB,
    data.game_size[0], data.game_size[1],
    0, GL_RGB,
    GL_UNSIGNED_BYTE,
    NULL    
  );
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    GL_TEXTURE_2D,
    data.framebuffer.texture,
    0
  );
  
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "Failed to setup framebuffer");
  }
  
  shader_use(data.framebuffer.shader);
  unsigned int u_framebuffer_texture = glGetUniformLocation(data.framebuffer.shader, "u_texture");
  // TODO: why set to 0 and not data.framebuffer.texture (which is 1)
  glUniform1i(u_framebuffer_texture, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // normal
  data.shader = shader_create(
    default_shader_vert_source,
    texture_shader_frag_source
  );

  glGenVertexArrays(1, &data.vao);
  glBindVertexArray(data.vao);

  glGenBuffers(1, &data.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct Vertex) * MAX_VERTICES, NULL, GL_DYNAMIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct Vertex),
    (const void*)offsetof(struct Vertex, position)
  );
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct Vertex),
    (const void*)offsetof(struct Vertex, local_position)
  );
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
    2,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct Vertex),
    (const void*)offsetof(struct Vertex, color)
  );
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(
    3,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(struct Vertex),
    (const void*)offsetof(struct Vertex, uv)
  );
  glEnableVertexAttribArray(4);
  glVertexAttribIPointer(
    4,
    1,
    GL_UNSIGNED_INT,
    sizeof(struct Vertex),
    (const void*)offsetof(struct Vertex, type)
  );
  
  unsigned int* indices = malloc(sizeof(unsigned int) * MAX_INDICES);
  for (int i = 0, j = 0; i < MAX_INDICES; i += 6, j += 4) {
    indices[i + 0] = j + 0;
    indices[i + 1] = j + 1;
    indices[i + 2] = j + 3;

    indices[i + 3] = j + 1;
    indices[i + 4] = j + 2;
    indices[i + 5] = j + 3;
  }

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    sizeof(unsigned int) * MAX_INDICES,
    indices,
    GL_STATIC_DRAW
  );
  free(indices);
  
  // matrices
  vec2 half_size = {size[0] / 2.0f, size[1] / 2.0f};
  glm_ortho(
    -half_size[0], half_size[0],
    -half_size[1], half_size[1],
    0.1f, 10.0f,
    data.projection
  );
  glm_lookat(
    (vec3){0.0f, 0.0f, 3.0f},
    (vec3){0.0f, 0.0f, 0.0f},
    (vec3){0.0f, 1.0f, 0.0f},
    data.view
  );
  
  shader_use(data.shader);  
  unsigned int u_projection = glGetUniformLocation(data.shader, "u_projection");
  unsigned int u_view = glGetUniformLocation(data.shader, "u_view");
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, (float*)data.projection);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, (float*)data.view);
}

void graphics_terminate() {
  shader_delete(data.shader);
  glfwDestroyWindow(data.window);
  glfwTerminate();
  free(data.vertices);
}

int graphics_should_close() {
  return glfwWindowShouldClose(data.window);
}

GLFWwindow* graphics_get_window() {
  return data.window;
}

float graphics_get_screen_scale() {
  int w, h;
  glfwGetWindowSize(data.window, &w, &h);
  return w / data.game_size[0];
}

float graphics_get_pixel_width() {
  return data.game_size[0];
}

float graphics_get_pixel_height() {
  return data.game_size[1];
}

void graphics_poll() {
  glfwPollEvents();
}

void graphics_start_draw() {
  glBindFramebuffer(GL_FRAMEBUFFER, data.framebuffer.fbo);
  glViewport(0, 0, data.game_size[0], data.game_size[1]);

  shader_use(data.shader);
  glBindVertexArray(data.vao);
  glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
  glBindTexture(GL_TEXTURE_2D, data.texture);  
}

void graphics_end_draw() {
  graphics_flush();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, data.window_size[0], data.window_size[1]);

  shader_use(data.framebuffer.shader);
  glBindVertexArray(data.framebuffer.vao);
  glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
  glBindTexture(GL_TEXTURE_2D, data.framebuffer.texture);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glfwSwapBuffers(data.window);
}

void graphics_clear(const float r, const float g, const float b) {
  glClearColor(r, g, b, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);  
}

void graphics_draw(const vec2 position, const vec2 uv, const vec2 size, const vec2 uv_size, const float rotation) {
  if (data.quad_count >= MAX_QUADS) {
    graphics_flush();
  }

  vec2 half_size;
  glm_vec2_divs(size, 2, half_size);
  vec2 uv_norm;
  vec2 uv_size_norm;
  glm_vec2_div(uv, data.texture_size, uv_norm);
  glm_vec2_div(uv_size, data.texture_size, uv_size_norm);

  size_t start = data.quad_count * 4;
  data.vertices[start + 0].type = 0;
  data.vertices[start + 0].position[0] = position[0] - half_size[0];
  data.vertices[start + 0].position[1] = position[1] - half_size[1];
  data.vertices[start + 0].uv[0] = uv_norm[0];
  data.vertices[start + 0].uv[1] = uv_norm[1] + uv_size_norm[1];
  glm_vec2_sub(data.vertices[start].position, position, data.vertices[start].position);
  glm_vec2_rotate(data.vertices[start].position, rotation, data.vertices[start].position);
  glm_vec2_add(data.vertices[start].position, position, data.vertices[start].position);
  
  data.vertices[start + 1].type = 0;
  data.vertices[start + 1].position[0] = position[0] - half_size[0];
  data.vertices[start + 1].position[1] = position[1] + half_size[1];
  data.vertices[start + 1].uv[0] = uv_norm[0];
  data.vertices[start + 1].uv[1] = uv_norm[1];
  glm_vec2_sub(data.vertices[start + 1].position, position, data.vertices[start + 1].position);
  glm_vec2_rotate(data.vertices[start + 1].position, rotation, data.vertices[start + 1].position);
  glm_vec2_add(data.vertices[start + 1].position, position, data.vertices[start + 1].position);

  data.vertices[start + 2].type = 0;
  data.vertices[start + 2].position[0] = position[0] + half_size[0];  
  data.vertices[start + 2].position[1] = position[1] + half_size[1];
  data.vertices[start + 2].uv[0] = uv_norm[0] + uv_size_norm[0];
  data.vertices[start + 2].uv[1] = uv_norm[1];
  glm_vec2_sub(data.vertices[start + 2].position, position, data.vertices[start + 2].position);
  glm_vec2_rotate(data.vertices[start + 2].position, rotation, data.vertices[start + 2].position);
  glm_vec2_add(data.vertices[start + 2].position, position, data.vertices[start + 2].position);
    
  data.vertices[start + 3].type = 0;
  data.vertices[start + 3].position[0] = position[0] + half_size[0];  
  data.vertices[start + 3].position[1] = position[1] - half_size[1];
  data.vertices[start + 3].uv[0] = uv_norm[0] + uv_size_norm[0];
  data.vertices[start + 3].uv[1] = uv_norm[1] + uv_size_norm[1];  
  glm_vec2_sub(data.vertices[start + 3].position, position, data.vertices[start + 3].position);
  glm_vec2_rotate(data.vertices[start + 3].position, rotation, data.vertices[start + 3].position);
  glm_vec2_add(data.vertices[start + 3].position, position, data.vertices[start + 3].position);
  
  data.quad_count += 1;
}

void graphics_draw_circle(const vec2 position, const float radius, const vec3 color) {
  if (data.quad_count >= MAX_QUADS) {
    graphics_flush();
  }
  
  float half_radius = radius / 2.0f;
  
  size_t start = data.quad_count * 4;
  data.vertices[start + 0].type = 1;
  data.vertices[start + 0].position[0] = position[0] - half_radius;
  data.vertices[start + 0].position[1] = position[1] - half_radius;
  data.vertices[start + 0].local_position[0] = -1.0f;
  data.vertices[start + 0].local_position[1] = -1.0f;
  glm_vec3_copy(color, data.vertices[start + 0].color);
  
  data.vertices[start + 1].type = 1;
  data.vertices[start + 1].position[0] = position[0] - half_radius;
  data.vertices[start + 1].position[1] = position[1] + half_radius;
  data.vertices[start + 1].local_position[0] = -1.0f;
  data.vertices[start + 1].local_position[1] = 1.0f;
  glm_vec3_copy(color, data.vertices[start + 1].color);

  data.vertices[start + 2].type = 1;
  data.vertices[start + 2].position[0] = position[0] + half_radius;  
  data.vertices[start + 2].position[1] = position[1] + half_radius;
  data.vertices[start + 2].local_position[0] = 1.0f;
  data.vertices[start + 2].local_position[1] = 1.0f;
  glm_vec3_copy(color, data.vertices[start + 2].color);
    
  data.vertices[start + 3].type = 1;
  data.vertices[start + 3].position[0] = position[0] + half_radius;  
  data.vertices[start + 3].position[1] = position[1] - half_radius;
  data.vertices[start + 3].local_position[0] = 1.0f;
  data.vertices[start + 3].local_position[1] = -1.0f;
  glm_vec3_copy(color, data.vertices[start + 3].color);

  data.quad_count += 1;
}
void graphics_draw_rect(const vec2 position, const vec2 size, const float rotation, const vec3 color) {
  if (data.quad_count >= MAX_QUADS) {
    graphics_flush();
  }
  
  vec2 half_size;
  glm_vec2_divs(size, 2, half_size);
  
  size_t start = data.quad_count * 4;
  data.vertices[start + 0].type = 2;
  data.vertices[start + 0].position[0] = position[0] - half_size[0];
  data.vertices[start + 0].position[1] = position[1] - half_size[1];
  glm_vec3_copy(color, data.vertices[start + 0].color);
  glm_vec2_sub(data.vertices[start + 0].position, position, data.vertices[start + 0].position);
  glm_vec2_rotate(data.vertices[start + 0].position, rotation, data.vertices[start + 0].position);
  glm_vec2_add(data.vertices[start + 0].position, position, data.vertices[start + 0].position);
  
  data.vertices[start + 1].type = 2;
  data.vertices[start + 1].position[0] = position[0] - half_size[0];
  data.vertices[start + 1].position[1] = position[1] + half_size[1];
  glm_vec3_copy(color, data.vertices[start + 1].color);
  glm_vec2_sub(data.vertices[start + 1].position, position, data.vertices[start + 1].position);
  glm_vec2_rotate(data.vertices[start + 1].position, rotation, data.vertices[start + 1].position);
  glm_vec2_add(data.vertices[start + 1].position, position, data.vertices[start + 1].position);

  data.vertices[start + 2].type = 2;
  data.vertices[start + 2].position[0] = position[0] + half_size[0];  
  data.vertices[start + 2].position[1] = position[1] - half_size[1];
  glm_vec3_copy(color, data.vertices[start + 2].color);
  glm_vec2_sub(data.vertices[start + 2].position, position, data.vertices[start + 2].position);
  glm_vec2_rotate(data.vertices[start + 2].position, rotation, data.vertices[start + 2].position);
  glm_vec2_add(data.vertices[start + 2].position, position, data.vertices[start + 2].position);
    
  data.vertices[start + 3].type = 2;
  data.vertices[start + 3].position[0] = position[0] + half_size[0];  
  data.vertices[start + 3].position[1] = position[1] + half_size[1];
  glm_vec3_copy(color, data.vertices[start + 3].color);
  glm_vec2_sub(data.vertices[start + 3].position, position, data.vertices[start + 3].position);
  glm_vec2_rotate(data.vertices[start + 3].position, rotation, data.vertices[start + 3].position);
  glm_vec2_add(data.vertices[start + 3].position, position, data.vertices[start + 3].position);

  data.quad_count += 1;
}

void graphics_draw_quad(const vec2 a, const vec2 b, const vec2 c, const vec2 d, const vec3 color) {
  
  if (data.quad_count >= MAX_QUADS) {
    graphics_flush();
  }
    
  size_t start = data.quad_count * 4;
  data.vertices[start + 0].type = 2;
  glm_vec2_copy(a, data.vertices[start + 0].position);
  glm_vec3_copy(color, data.vertices[start + 0].color);
  
  data.vertices[start + 1].type = 2;
  glm_vec2_copy(b, data.vertices[start + 1].position);
  glm_vec3_copy(color, data.vertices[start + 1].color);

  data.vertices[start + 2].type = 2;
  glm_vec2_copy(c, data.vertices[start + 2].position);
  glm_vec3_copy(color, data.vertices[start + 2].color);
    
  data.vertices[start + 3].type = 2;
  glm_vec2_copy(d, data.vertices[start + 3].position);
  glm_vec3_copy(color, data.vertices[start + 3].color);

  data.quad_count += 1;
}

