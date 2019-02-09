#include <drb_graphics.h>

#include <cstdio>

#include "gui.h"
#include "input.h"
#include "log.h"
#include "network.h"
#include "scene.h"
#include "window.h"

int main(int argc, char** argv) {
  {
    log_enable_trace();
    log_enable_warning();
    log_enable_error();
    log_enable_all_categories();
  }

  struct window window;
  struct input input;
  if (!window_create(&window, "Round table", 0.0f, 0.0f, 1366.0f, 768.0f)) {
    log_error(LOG_MAIN, "ERROR: Failed to create window.\n");
    return 1;
  }

  gfx_init();
  gui_init(&window);
  input_init(&input, &window);

  struct scene scene = (struct scene){};
  scene_init(&scene, &window);

  struct renderer renderer;
  if (!renderer_init(&renderer)) {
    log_error(LOG_MAIN, "ERROR: Failed to initialize renderer.\n");
    return 1;
  }

  struct net net;
  if (!net_init(&net)) {
    log_error(LOG_MAIN, "ERROR: Failed to initialize network.\n");
  }

  float prev_time = window_get_time(&window);
  while (!window_should_close(&window)) {
    float time = window_get_time(&window);
    float dt = time - prev_time;
    prev_time = time;

    gui_prepare(&window);
    scene_process_gui(&scene, &renderer, &window);
    renderer_process_gui(&renderer);
    net_process_gui(&net);
    input_update_mouse_position(&window, &input);

    if (!gui_wants_input()) {
      if (input.mouse_left_pressed)
        log_trace(LOG_INPUT, "LMB pressed\n");
      if (input.w_pressed)
        log_trace(LOG_INPUT, "W pressed\n");
    }

    scene_process_input(dt, &scene, &input);

    net_update(&net);

    gfx_clear_screen((v3){.x = 0.0f, .y = 0.0f, .z = 0.0f});
    scene_draw(&scene, &renderer);
    gui_draw();
    window_swap_buffers(&window);
  }

  gui_shutdown();
  renderer_terminate(&renderer);
  window_destroy(&window);
}