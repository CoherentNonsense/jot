#ifndef INPUT_H
#define INPUT_H

#include "types.h"

#include <GLFW/glfw3.h>

struct JotKeyData {
  int state;
  int down;
  int up;
};

struct JotInputData {
  GLFWwindow* window;
  struct JotKeyData keys[KeyCodeCount];
};

void input_set_callback(GLFWwindow* window);
int input_key(enum KeyCode key);
int input_key_down(enum KeyCode key);
int input_key_up(enum KeyCode key);

void input_update();

#endif