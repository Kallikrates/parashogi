#include "common.hpp"
#include "render.hpp"

#include <thread>
#include <future>

static GLFWwindow * window_actual;
GLFWwindow * const & window = window_actual;

int main() {
	glfwInit();
	window_actual = glfwCreateWindow(800, 600, "Parabolic Shogi", nullptr, nullptr);
	std::future<int> rexec = std::async(std::launch::async, render::exec);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return rexec.get();
}
