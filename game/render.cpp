#include <GL/glew.h>
#include "common.hpp"
#include "render.hpp"

int render::exec() {
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
	}
}
