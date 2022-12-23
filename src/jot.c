#include "jot.h"

#include "graphics.h"
#include "input.h"

#include <cglm/cam.h>

void jot_init(const char* title, const int width, const int height) {
  graphics_init(title, (vec2){width, height});
  input_set_callback(graphics_get_window());
}

void jot_terminate() {
  graphics_terminate();
}

float jot_get_time() {
  return glfwGetTime();
}

bool jot_update() {
  // update display
  graphics_end_draw();
  
  // update input
  graphics_poll();
  input_update();

  // get ready for next frame
  graphics_start_draw();
  
  return !graphics_should_close();
}

void jot_load_tex(const char* path) {
  unsigned int texture = graphics_load_texture(path);
  graphics_use_texture(texture);
}

void jot_clear(const float r, const float g, const float b) {
  graphics_clear(r, g, b);
}

void jot_draw_sprite(const float u, const float v, const float uv_width, const float uv_height, const float x, const float y, const float width, const float height, const float rotation) {
  graphics_draw((vec2){x, y}, (vec2){u, v}, (vec2){uv_width, uv_height}, rotation);
}

void jot_draw_circle(const float x, const float y, const float radius, const float r, const float g, const float b) {
  graphics_draw_circle((vec2){x, y}, (float)radius, (vec3){r, g, b});
}

// input
// -----
bool jot_get_key(enum KeyCode key) {
  return input_key(key);
}

bool jot_get_key_down(enum KeyCode key) {
  return input_key_down(key);
}

bool jot_get_key_up(enum KeyCode key) {
  return input_key_up(key);
}
