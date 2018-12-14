#include <GLFW/glfw3.h>

#include <drb_graphics.h>

#include <stdio.h>

struct window
{
	GLFWwindow* glfw_win;
};

bool create_window(struct window* window, const char* title, float x, float y, float width, float height)
{
	GLFWwindow* glfw_win = 0;
	{ // Setup GLFW along with OpenGL context
	    if (!glfwInit()) {
	    	return false;
	    }

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfw_win = glfwCreateWindow(width, height, title, NULL, NULL);

		if (!glfw_win)
	    {
	        glfwTerminate();
	        return false;
	    }
	    glfwSetWindowPos(glfw_win, x, y);
	    
	    glfwMakeContextCurrent(glfw_win);
	    glfwSwapInterval(1);
	}

	gfx_init();

	window->glfw_win = glfw_win;

    return true;
}

void destroy_window(struct window* window)
{
	glfwDestroyWindow(window->glfw_win);
    glfwTerminate();
}

int main(int argc, char** argv)
{
	struct window win;
	if(!create_window(&win, "Round table", 0.0f, 0.0f, 1366.0f, 768.0f))
	{
		fprintf(stderr, "ERROR: Failed to create window.\n");
		return 1;
	}

	// float prev_time = glfwGetTime();
	while(!glfwWindowShouldClose(win.glfw_win))
    {
    	// float time = glfwGetTime();
    	// float dt = time - prev_time;
    	// prev_time = time;

    	// ???
    	// profit

		glfwSwapBuffers(win.glfw_win);
		glfwPollEvents();
    }

    destroy_window(&win);
}