#ifndef COMMON_HPP
#define COMMON_HPP

#include <atomic>
#include <cstdint>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

extern GLFWwindow * const & window;
extern std::atomic_int req_width, req_height;

#endif //COMMON_HPP
