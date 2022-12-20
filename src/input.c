#include "input.h"
#include <stdio.h>

static struct JotInputData data;

static void set_key(enum KeyCode keycode, int state) {
  if (keycode == -1)
    return;
  
  struct JotKeyData* key = &data.keys[keycode];
  key->state = state;
  key->up = 1 - state;
  key->down = state;
}

static enum KeyCode convert_key(int key) {
  if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
    return key - GLFW_KEY_A + JOT_KEY_A;
  
  switch (key) {
    case GLFW_KEY_UP: return JOT_KEY_UP;
    case GLFW_KEY_DOWN: return JOT_KEY_DOWN;
    case GLFW_KEY_LEFT: return JOT_KEY_LEFT;
    case GLFW_KEY_RIGHT: return JOT_KEY_RIGHT;
    case GLFW_KEY_SPACE: return JOT_KEY_SPACE;
    default: return -1;
  }
}

static void key_callback(GLFWwindow* window, int keycode, int scancode, int action, int mods) {
  switch (action) {
    case GLFW_PRESS:
      set_key(convert_key(keycode), 1);
      break;
    case GLFW_RELEASE:
      set_key(convert_key(keycode), 0);
      break;
    default:
      break;
      
  }
}

void input_set_callback(GLFWwindow* window) {
  glfwSetKeyCallback(window, key_callback);
}

int input_key(enum KeyCode keycode) {
  return data.keys[keycode].state;
}

int input_key_down(enum KeyCode keycode) {
  return data.keys[keycode].down;
}

int input_key_up(enum KeyCode keycode) {
  return data.keys[keycode].up;
}

void input_update() {
  for (int i = 0; i < KeyCodeCount; ++i) {
    struct JotKeyData* key = &data.keys[i];
    key->down = 0;
    key->up = 0;
  }
}