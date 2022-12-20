#ifndef JOT_TYPES_H
#define JOT_TYPES_H

typedef unsigned int jot_aud;
typedef unsigned int jot_color;

#define JOT_RED 0
#define JOT_BLUE 1

enum KeyCode {
  JOT_KEY_A, JOT_KEY_B, JOT_KEY_C, JOT_KEY_D, JOT_KEY_E,
  JOT_KEY_F, JOT_KEY_G, JOT_KEY_H, JOT_KEY_I, JOT_KEY_J,
  JOT_KEY_K, JOT_KEY_L, JOT_KEY_M, JOT_KEY_N, JOT_KEY_O,
  JOT_KEY_P, JOT_KEY_Q, JOT_KEY_R, JOT_KEY_S, JOT_KEY_T,
  JOT_KEY_U, JOT_KEY_V, JOT_KEY_W, JOT_KEY_X, JOT_KEY_Y,
  JOT_KEY_Z,

  JOT_KEY_UP, JOT_KEY_DOWN, JOT_KEY_LEFT, JOT_KEY_RIGHT,
  
  JOT_KEY_SPACE,
  
  KeyCodeCount,
};

#endif