#include "window.h"

#include <GLFW/glfw3.h>

bool window_create(struct window* window, const char* title, float x, float y,
                   float width, float height) {
  GLFWwindow* glfw_win = 0;
  { // Setup GLFW along with OpenGL context
    if (!glfwInit()) {
      return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfw_win = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!glfw_win) {
      glfwTerminate();
      return false;
    }
    glfwSetWindowPos(glfw_win, x, y);

    glfwMakeContextCurrent(glfw_win);
    glfwSwapInterval(1);
  }

  window->glfw_win = glfw_win;
  window->x = x;
  window->y = y;
  window->width = width;
  window->height = height;

  return true;
}

void window_destroy(struct window* window) {
  glfwDestroyWindow(window->glfw_win);
  glfwTerminate();
}

void window_swap_buffers(struct window* window) {
  glfwSwapBuffers(window->glfw_win);
  glfwPollEvents();
}

bool window_should_close(struct window* window) {
  return glfwWindowShouldClose(window->glfw_win);
}