#include "common.hpp"
#include "render_common.hpp"
#include "render.hpp"
#include "render_textures.hpp"
#include "render_shaders.hpp"
#include "render_meshes.hpp"
#include "render_framebuffers.hpp"

#include "game.hpp"

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
	if (!render::framebuffer::init(cur_width, cur_height)) ERRRET(1, "FATAL: Framebuffers failed to initialize.\n");

	glClearColor(0.0f, 0.75f, 1.0f, 1.0f);

	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)) {

		//Adjust Viewport and Framebuffers if necessary

		if (cur_width != req_width || cur_height != req_height) {
			cur_width = req_width;
			cur_height = req_height;
			glViewport(0, 0, cur_width, cur_height);
			render::framebuffer::update_framebuffer_sizes(cur_width, cur_height);
		}

		//Bind the color+glow framebuffer
		render::framebuffer::cc->bind();
		render::framebuffer::clear();

		//Draw the board
		render::shader::board->use();
		render::texture::bind_board();
		render::shader::board->uniform_transform(glm::vec3{0, 0, 0}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
		render::mesh::draw_board();

		//Draw the pieces
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		render::texture::bind_piece_pawn();
		render::shader::piece->use();
		game::set_board_lock(true);
		for (shogi::piece_placement const & piece : game::get_pieces()) {
			if (piece.piece->align == shogi::alignment::HOME) {
				render::shader::piece->uniform_color(glm::vec3{1.0f, 0.0f, 0.5f});
			} else {
				render::shader::piece->uniform_color(glm::vec3{0.0f, 1.0f, 0.5f});
			}
			render::shader::piece->uniform_transform(glm::vec3{piece.coords.x / 4.5f - (1.0f - (1.0f / 9)), piece.coords.y / 4.5f - (1.0f - (1.0f / 9)), 0}, glm::vec3{0.0f, 0.0f, piece.piece->align == shogi::alignment::HOME ? 0.0f : glm::radians(180.0f)}, glm::vec3{0.111111f, 0.111111f, 1.0f});
			render::mesh::draw_piece();
		}
		game::set_board_lock(false);
		glDisable(GL_BLEND);

		//Bind main framebuffer and begin first pass - diffuse
		render::framebuffer::use_mainfb();
		render::shader::postproc_0->use();
		render::framebuffer::cc->bind_as_textures();
		render::mesh::draw_fullquad();

		//Second pass - glow
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_COLOR, GL_ONE, GL_ONE, GL_ZERO);
		render::shader::postproc_glow->use();
		render::mesh::draw_fullquad();
		glDisable(GL_BLEND);

		glfwSwapBuffers(window);
	}
}
