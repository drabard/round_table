#pragma once

#include <stdint.h>

struct window;

struct input {
  double mouse_x;
  double mouse_y;
  double mouse_dx;
  double mouse_dy;

  uint8_t mouse_left_pressed : 1;
  uint8_t mouse_right_pressed : 1;

  uint8_t w_pressed : 1;
  uint8_t s_pressed : 1;
  uint8_t a_pressed : 1;
  uint8_t d_pressed : 1;
  uint8_t q_pressed : 1;
  uint8_t e_pressed : 1;
  uint8_t c_pressed : 1;
  uint8_t z_pressed : 1;
  uint8_t r_pressed : 1;
  uint8_t f_pressed : 1;
};

void input_init(struct input* input, struct window* window);
void input_update_mouse_position(struct window* window, struct input* input);
