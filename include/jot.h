#ifndef JOT_H
#define JOT_H

#include <stdbool.h>

enum KeyCode {
  JOT_KEY_A, JOT_KEY_B, JOT_KEY_C, JOT_KEY_D, JOT_KEY_E,
  JOT_KEY_F, JOT_KEY_G, JOT_KEY_H, JOT_KEY_I, JOT_KEY_J,
  JOT_KEY_K, JOT_KEY_L, JOT_KEY_M, JOT_KEY_N, JOT_KEY_O,
  JOT_KEY_P, JOT_KEY_Q, JOT_KEY_R, JOT_KEY_S, JOT_KEY_T,
  JOT_KEY_U, JOT_KEY_V, JOT_KEY_W, JOT_KEY_X, JOT_KEY_Y,
  JOT_KEY_Z,

  JOT_KEY_UP, JOT_KEY_DOWN, JOT_KEY_LEFT, JOT_KEY_RIGHT,
  
  JOT_KEY_SPACE, JOT_KEY_ENTER, JOT_KEY_TAB,
  JOT_KEY_LSHIFT, JOT_KEY_RSHIFT,
  
  JOT_LMOUSE, JOT_RMOUSE,
  
  KeyCodeCount,
};


// core
// ----
void jot_init(const char* title, const int width, const int height);
void jot_terminate();

bool jot_update();
float jot_get_time();

// assets
// ------
void jot_texture(const char* path);
// jot_aud jot_load_mp3(const char* path);

// drawing
// -------
void jot_clear(const float r, const float g, const float b);
void jot_draw_sprite(const float u, const float v, const float uv_width, const float uv_height,  const float x, const float y, const float width, const float height, const float rotation);
void jot_draw_circle(const float x, const float y, const float radius, const float r, const float g, const float b);
void jot_draw_rect(const float x, const float y, const float width, const float height, const float rotation, const float r, const float g, const float b);

void jot_draw_quad(
  const float x_1, const float y_1,
  const float x_2, const float y_2,
  const float x_3, const float y_3,
  const float x_4, const float y_4,
  const float r, const float g, const float b
);

void jot_draw_line(
  const float x_1, const float y_1, const float width_1,
  const float x_2, const float y_2, const float width_2,
  const float r, const float g, const float b
);

// input
// -----
bool jot_key(enum KeyCode key);
bool jot_key_up(enum KeyCode key);
bool jot_key_down(enum KeyCode key);
float jot_cursor_x();
float jot_cursor_y();

// audio (TODO)
// -----
// void jot_play(jot_aud audio);

#endif