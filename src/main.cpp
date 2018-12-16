#include <drb_graphics.h>

#include <stdio.h>

#include "window.h"
#include "gui.h"

int main(int argc, char** argv)
{
	struct window window;
	if(!window_create(&window, "Round table", 0.0f, 0.0f, 1366.0f, 768.0f))
	{
		fprintf(stderr, "ERROR: Failed to create window.\n");
		return 1;
	}

	gfx_init();
	gui_init(&window);

	// float prev_time = glfwGetTime();
	while(!window_should_close(&window))
    {
    	// float time = glfwGetTime();
    	// float dt = time - prev_time;
    	// prev_time = time;

    	gui_prepare(&window);
    	gui_draw();

    	window_swap_buffers(&window);
    }

    gui_shutdown();
    window_destroy(&window);
}