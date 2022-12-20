#include "shader.h"

#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>

static const char* read_file(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    return NULL;
  }
  
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  
  char* buffer = malloc(file_size + 1);
  if (buffer == NULL) {
    return NULL;
  }
  
  rewind(file);
  fread(buffer, file_size, 1, file);
  fclose(file);
  buffer[file_size] = '\0';
  return buffer;
}

static unsigned int compile_shader(const char* shader_source, GLenum type) {  
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &shader_source, NULL);
  glCompileShader(shader);
  
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    if (type == GL_VERTEX_SHADER) {
      fprintf(stderr, "VERTEX ");
    } else {
      fprintf(stderr, "FRAGMENT ");
    }
    fprintf(stderr, "%s", info_log);
  }
  
  return shader;
}

unsigned int shader_create(const char* vertex_source, const char* fragment_source) {
  unsigned int vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER);
  unsigned int fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER);
  
  unsigned int program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  
  int success;
  char info_log[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, info_log);
    fprintf(stderr, "Link Error: %s", info_log);
  }
  
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  
  return program;
}

unsigned int shader_create_path(const char* vertex_path, const char* fragment_path) {
  const char* vertex_source = read_file(vertex_path);  
  if (vertex_source == NULL) {
    fprintf(stderr, "Could not load vertex shader");
    return 0;
  }
  
  const char* fragment_source = read_file(fragment_path);
  if (fragment_source == NULL) {
    fprintf(stderr, "Could not load fragment shader");
    return 0;
  }
  
  return shader_create(vertex_source, fragment_source);
}

void shader_delete(unsigned int shader) {
  glDeleteProgram(shader);
}

void shader_use(unsigned int shader) {
  glUseProgram(shader);
}