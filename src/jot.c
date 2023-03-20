#include "jot.h"

#include "graphics.h"
#include "input.h"

#include <cglm/cam.h>
#include <math.h>
#include <stdio.h>

void open_window(const char* title, const int width, const int height) {
  graphics_init(title, (vec2){width, height});
  input_set_callback(graphics_get_window());
}

void close_window() {
  graphics_terminate();
}

float get_time() {
  return glfwGetTime();
}

bool update_window() {
  // update display
  graphics_end_draw();
  
  // update input
  graphics_poll();
  input_update();

  // get ready for next frame
  graphics_start_draw();
  
  return !graphics_should_close();
}

void load_texture(const char* path) {
  unsigned int texture = graphics_load_texture(path);
  graphics_use_texture(texture);
}

void clear_screen(const float r, const float g, const float b) {
  graphics_clear(r, g, b);
}

void draw_sprite(const float u, const float v, const float uv_width, const float uv_height, const float x, const float y, const float width, const float height, const float rotation) {
  graphics_draw((vec2){x, y}, (vec2){u, v}, (vec2){width, height}, (vec2){uv_width, uv_height}, rotation);
}

// TODO: Make graphics_draw_circle -> graphics_draw_ellipse
void fill_ellipse(const float x, const float y, const float width, const float height, const float rotation, const float r, const float g, const float b) {
  graphics_draw_circle((vec2){x, y}, (float)width / 2.0f, (vec3){r, g, b});
}

void fill_circle(const float x, const float y, const float radius, const float r, const float g, const float b) {
  graphics_draw_circle((vec2){x, y}, (float)radius, (vec3){r, g, b});
}

void fill_rect(const float x, const float y, const float width, const float height, const float rotation, const float r, const float g, const float b) {
  graphics_draw_rect((vec2){x, y}, (vec2){width, height}, rotation, (vec3){r, g, b});
}

void fill_quad(
  const float x_1, const float y_1,
  const float x_2, const float y_2,
  const float x_3, const float y_3,
  const float x_4, const float y_4,
  const float r, const float g, const float b
) {
  graphics_draw_quad((vec2){x_1, y_1}, (vec2){x_2, y_2}, (vec2){x_3, y_3}, (vec2){x_4, y_4}, (vec3){r, g, b});
}

void draw_line(
  const float x_1, const float y_1, const float width_1,
  const float x_2, const float y_2, const float width_2,
  const float r, const float g, const float b
) {
  float angle = atan2f(y_2 - y_1, x_2 - x_1);
  graphics_draw_quad(
    (vec2){x_1 - width_1 / 2 * sinf(angle), y_1 + width_1 / 2 * cosf(angle)},
    (vec2){x_1 + width_1 / 2 * sinf(angle), y_1 - width_1 / 2 * cosf(angle)},
    (vec2){x_2 + width_2 / 2 * sinf(angle), y_2 - width_2 / 2 * cosf(angle)},
    (vec2){x_2 - width_2 / 2 * sinf(angle), y_2 + width_2 / 2 * cosf(angle)},
    (vec3){r, g, b}
  );
}

// input
// -----
bool get_key(enum KeyCode key) {
  return input_key(key);
}

bool get_key_down(enum KeyCode key) {
  return input_key_down(key);
}

bool get_key_up(enum KeyCode key) {
  return input_key_up(key);
}

float cursor_x() {
  return input_cursor_x() / graphics_get_screen_scale();
};

float cursor_y() {
  return graphics_get_pixel_height() - input_cursor_y() / graphics_get_screen_scale();
};