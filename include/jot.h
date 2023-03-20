#ifndef JOT_H
#define JOT_H

#include <stdbool.h>

enum KeyCode {
  KEY_A, KEY_B, KEY_C, KEY_D, KEY_E,
  KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
  KEY_K, KEY_L, KEY_M, KEY_N, KEY_O,
  KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
  KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y,
  KEY_Z,

  KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
  
  KEY_SPACE, KEY_ENTER, KEY_TAB,
  KEY_LSHIFT, KEY_RSHIFT,
  
  LMOUSE, RMOUSE,
  
  KeyCodeCount,
};


/*
 * \------------------\
 * \  core functions  \
 * \------------------\
*/

void open_window(const char* title, const int width, const int height);
void close_window();

bool update_window();
float get_time();

void load_texture(const char* path);

bool get_key(enum KeyCode key);
bool get_key_up(enum KeyCode key);
bool get_key_down(enum KeyCode key);
float cursor_x();
float cursor_y();


/*
 * \---------------------\
 * \  drawing functions  \
 * \---------------------\
*/

/*
 * Fills the screen with the color (r, g, b)
 */
void clear_screen(const float r, const float g, const float b);

/**
 * Draws a sprite from the loaded texture from (u, v) to (x, y) with a size from (uv_width, uv_height) to (width, height) and a rotation
 */
void draw_sprite(
  const float u, const float v, /* uv coordinate */
  const float uv_width, const float uv_height, /* uv size */
  const float x, const float y, /* position */
  const float width, const float height, /* size */
  const float rotation /* rotation */
);

/**
 * Draws a filled ellipse at position (x, y) with size (width, height) and rotation with color (r, g, b)
 */
void fill_ellipse(
  const float x, const float y,
  const float width, const float height,
  const float rotation,
  const float r, const float g, const float b
);

/**
 * Draws a filled circle at position (x, y) with a radius and color (r, g, b)
 */
void fill_circle(
  const float x, const float y,
  const float radius,
  const float r, const float g, const float b
);

/**
 * Draws a filled rectangle at position (x, y) with a width, height, and rotation and color (r, g, b)
 */
void fill_rect(
  const float x, const float y,
  const float width, const float height,
  const float rotation,
  const float r, const float g, const float b
);

/**
 * Draws a filled quadrilateral with points (x_1, y_1), (x_2, y_2), (x_3, y_3), and (x_4, y_4) and color (r, g, b)
 */
void fill_quad(
  const float x_1, const float y_1,
  const float x_2, const float y_2,
  const float x_3, const float y_3,
  const float x_4, const float y_4,
  const float r, const float g, const float b
);

/**
 * Draws a line from (x_1, y_1) with width_1 to (x_2, y_2) with width_2 and color (r, g, b)
 */
void draw_line(
  const float x_1, const float y_1, const float width_1,
  const float x_2, const float y_2, const float width_2,
  const float r, const float g, const float b
);

#endif