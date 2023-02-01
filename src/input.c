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
  key->event_tick = 1;
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
    case GLFW_KEY_LEFT_SHIFT: return JOT_KEY_LSHIFT;
    case GLFW_KEY_RIGHT_SHIFT: return JOT_KEY_RSHIFT;
    case GLFW_KEY_ENTER: return JOT_KEY_ENTER;
    case GLFW_KEY_TAB: return JOT_KEY_TAB;
    
    case GLFW_MOUSE_BUTTON_LEFT: return JOT_LMOUSE;
    case GLFW_MOUSE_BUTTON_RIGHT: return JOT_RMOUSE;
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

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
  key_callback(window, button, 0, action, mods);
}

void input_set_callback(GLFWwindow* window) {
  data.window = window;
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_callback);
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

float input_cursor_x() {
  return (float)data.cursor_x;
}

float input_cursor_y() {
  return (float)data.cursor_y;
}

void input_update() {
  for (int i = 0; i < KeyCodeCount; ++i) {
    struct JotKeyData* key = &data.keys[i];
    if (!key->event_tick) {
      key->down = 0;
      key->up = 0;
    } else {
      key->event_tick = 0;
    }
  }
  
  glfwGetCursorPos(data.window, &data.cursor_x, &data.cursor_y);
}