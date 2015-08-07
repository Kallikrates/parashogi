#include "render_meshes.hpp"
#include "render_common.hpp"

static GLfloat const fs_v [] = {
	-1.0,	-1.0,	0.0,
	-1.0,	 1.0,	0.0,
	 1.0,	-1.0,	0.0,
	 1.0,	 1.0,	0.0,
};
static GLfloat const fs_uv [] = {
	 0.0,	 0.0,
	 0.0,	 1.0,
	 1.0,	 0.0,
	 1.0,	 1.0,
};

static GLuint board_vao, board_vbuf, board_uvbuf;

bool render::mesh::init() {
	glCreateVertexArrays(1, &board_vao);

	glCreateBuffers(1, &board_vbuf);
	glNamedBufferData(board_vbuf, sizeof(fs_v), fs_v, GL_STATIC_DRAW);

	glVertexArrayAttribBinding(board_vao, 0, 0);
	glVertexArrayVertexBuffer(board_vao, 0, board_vbuf, 0, 12);
	glEnableVertexArrayAttrib(board_vao, 0);
	glVertexArrayAttribFormat(board_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glCreateBuffers(1, &board_uvbuf);
	glNamedBufferData(board_uvbuf, sizeof(fs_uv), fs_uv, GL_STATIC_DRAW);

	glVertexArrayAttribBinding(board_vao, 1, 1);
	glVertexArrayVertexBuffer(board_vao, 1, board_uvbuf, 0, 8);
	glEnableVertexArrayAttrib(board_vao, 1);
	glVertexArrayAttribFormat(board_vao, 1, 2, GL_FLOAT, GL_FALSE, 0);

	return true;
}

void render::mesh::draw_board() {
	glBindVertexArray(board_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void render::mesh::draw_piece() {
	glBindVertexArray(board_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void render::mesh::draw_fullquad() {
	glBindVertexArray(board_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
