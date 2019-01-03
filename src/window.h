#pragma once

struct GLFWwindow;

struct window {
  GLFWwindow* glfw_win;

  float x;
  float y;
  float width;
  float height;
};

bool window_create(struct window* window, const char* title, float x, float y,
                   float width, float height);
bool window_should_close(struct window* window);
void window_swap_buffers(struct window* window);
void window_destroy(struct window* window);