#include "common.hpp"
#include "render_common.hpp"
#include "render.hpp"
#include "render_textures.hpp"
#include "render_shaders.hpp"
#include "render_meshes.hpp"

#include <cstdio>

#define ERRRET( code, msg... ) {printf(msg); glfwSetWindowShouldClose(window, 1); return code;}

static int cur_width, cur_height;

int render::exec() {
	cur_width = req_width;
	cur_height = req_height;
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) ERRRET(1, "FATAL: GLEW could not be initialized!\n");
	if (!GLEW_VERSION_4_5) ERRRET(1, "FATAL: This program requires an OpenGL 4.5 compatible render device.\n");
	if (!render::shader::init()) ERRRET(1, "FATAL: Shaders failed to load.\n");
	if (!render::texture::init()) ERRRET(1, "FATAL: Textures failed to load.\n");
	if (!render::mesh::init()) ERRRET(1, "FATAL: Meshes failed to load.\n");

	glClearColor(0.0f, 0.75f, 1.0f, 1.0f);

	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		if (cur_width != req_width || cur_height != req_height) {
			cur_width = req_width;
			cur_height = req_height;
			glViewport(0, 0, cur_width, cur_height);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render::shader::use_board_program();

		render::texture::bind_board(0);
		render::shader::uniform_transform("trans", glm::vec3{0, 0, 0}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
		render::mesh::draw_board();

		render::texture::bind_piece_pawn(0);
		render::shader::uniform_transform("trans", glm::vec3{-1, -1, 0}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.5f, 0.5f, 0.5f});
		render::mesh::draw_board();

		glfwSwapBuffers(window);
	}
}
