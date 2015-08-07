#include "common.hpp"
#include "render_common.hpp"
#include "render.hpp"
#include "game.hpp"

#include <thread>
#include <future>

static GLFWwindow * window_actual;
GLFWwindow * const & window = window_actual;

std::atomic_int req_width {1024}, req_height {1024};

static void gl_resize(GLFWwindow * window, int w, int h) {
	req_width = w;
	req_height = h;
}

int main() {
	glfwInit();
	window_actual = glfwCreateWindow(req_width, req_height, "Parabolic Shogi", nullptr, nullptr);
	glfwSetWindowSizeCallback(window, gl_resize);
	game::init();
	std::future<int> rexec = std::async(std::launch::async, render::exec);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return rexec.get();
}
