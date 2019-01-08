#include "input.h"
#include "window.h"

#include <GLFW/glfw3.h>

static void input_key_callback(GLFWwindow* window, int key, int scancode,
                               int action, int mods) {
#define SET_KEY(key_id, input_byte)                                            \
  if (key == key_id) {                                                         \
    switch (action) {                                                          \
    case GLFW_PRESS:                                                           \
      input_byte = 1;                                                          \
      break;                                                                   \
    case GLFW_RELEASE:                                                         \
      input_byte = 0;                                                          \
      break;                                                                   \
    }                                                                          \
  }

  struct input* input = (struct input*)glfwGetWindowUserPointer(window);

  SET_KEY(GLFW_KEY_W, input->w_pressed);
  SET_KEY(GLFW_KEY_S, input->s_pressed);
  SET_KEY(GLFW_KEY_A, input->a_pressed);
  SET_KEY(GLFW_KEY_D, input->d_pressed);
  SET_KEY(GLFW_KEY_Q, input->q_pressed);
  SET_KEY(GLFW_KEY_E, input->e_pressed);
  SET_KEY(GLFW_KEY_C, input->c_pressed);
  SET_KEY(GLFW_KEY_Z, input->z_pressed);
  SET_KEY(GLFW_KEY_R, input->r_pressed);
  SET_KEY(GLFW_KEY_F, input->f_pressed);
}

static void input_mouse_button_callback(GLFWwindow* window, int button,
                                        int action, int mods) {
#define SET_BUTTON(key_id, input_byte)                                         \
  if (button == key_id) {                                                      \
    switch (action) {                                                          \
    case GLFW_PRESS:                                                           \
      input_byte = 1;                                                          \
      break;                                                                   \
    case GLFW_RELEASE:                                                         \
      input_byte = 0;                                                          \
      break;                                                                   \
    }                                                                          \
  }

  struct input* input = (struct input*)glfwGetWindowUserPointer(window);

  SET_BUTTON(GLFW_MOUSE_BUTTON_RIGHT, input->mouse_right_pressed);
  SET_BUTTON(GLFW_MOUSE_BUTTON_LEFT, input->mouse_left_pressed);
}

void input_init(struct input* input, struct window* window) {
  *input = (struct input){};
  glfwSetKeyCallback(window->glfw_win, input_key_callback);
  glfwSetMouseButtonCallback(window->glfw_win, input_mouse_button_callback);
  glfwSetWindowUserPointer(window->glfw_win, input);
}

void input_update_mouse_position(struct window* window, struct input* input) {
  double old_x = input->mouse_x;
  double old_y = input->mouse_y;
  glfwGetCursorPos(window->glfw_win, &input->mouse_x, &input->mouse_y);
  input->mouse_dx = input->mouse_x - old_x;
  input->mouse_dy = input->mouse_y - old_y;
}
