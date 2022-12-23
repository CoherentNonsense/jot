#ifndef JOT_H
#define JOT_H

#include "types.h"

#include <stdbool.h>

// core
// ----
void jot_init(const char* title, const int width, const int height);
void jot_terminate();

bool jot_update();
float jot_get_time();

// assets
// ------
void jot_load_tex(const char* path);
// jot_aud jot_load_mp3(const char* path);

// drawing
// -------
void jot_clear(const float r, const float g, const float b);
void jot_draw_sprite(const float u, const float v, const float uv_width, const float uv_height,  const float x, const float y, const float width, const float height, const float rotation);
void jot_draw_circle(const float x, const float y, const float radius, const float r, const float g, const float b);
// void jot_draw_line(const int x_1, const int y_1, const int width_1, const jot_color color_2, const int x_2, const int y_2, const int width_2);
// void jot_draw_square(const int x, const int y, const int width, const int height, const jot_color color);

// input
// -----
bool jot_get_key(enum KeyCode key);
bool jot_get_key_up(enum KeyCode key);
bool jot_get_key_down(enum KeyCode key);

// audio (TODO)
// -----
// void jot_play(jot_aud audio);

#endif