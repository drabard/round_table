#include <drb_graphics.h>

#include <cstdio>

#include "gui.h"
#include "input.h"
#include "scene.h"
#include "window.h"

int main(int argc, char** argv) {
  struct window window;
  struct input input;
  if (!window_create(&window, "Round table", 0.0f, 0.0f, 1366.0f, 768.0f)) {
    fprintf(stderr, "ERROR: Failed to create window.\n");
    return 1;
  }

  gfx_init();
  gui_init(&window);
  input_init(&input, &window);

  struct scene scene = (struct scene){};

  struct renderer renderer;
  if (!renderer_init(&renderer)) {
    fprintf(stderr, "ERROR: Failed to initialize renderer.");
    return 1;
  }

  // float prev_time = glfwGetTime();
  while (!window_should_close(&window)) {
    // float time = glfwGetTime();
    // float dt = time - prev_time;
    // prev_time = time;

    gui_prepare(&window);
    scene_process_gui(&scene, &renderer, &window);
    renderer_process_gui(&renderer);
    input_update_mouse_position(&window, &input);

    if (!gui_wants_input()) {
      if (input.mouse_left_pressed)
        printf("LMB pressed\n");
      if (input.w_pressed)
        printf("W pressed\n");
    }

    gfx_clear_screen((v3){.x = 0.0f, .y = 0.0f, .z = 0.0f});
    scene_draw(&scene, &renderer);
    gui_draw();
    window_swap_buffers(&window);
  }

  gui_shutdown();
  renderer_terminate(&renderer);
  window_destroy(&window);
}