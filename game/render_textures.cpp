#include "render_textures.hpp"
#include "render_loadpng.hpp"

#include <cstdio>

static GLuint board_texture = 0;
static GLuint piece_pawn_texture = 0;

static inline void bind_texture(GLuint texture, int binding) {
	switch(binding) {
	case 0:
		glActiveTexture(GL_TEXTURE0);
		break;
	case 1:
		glActiveTexture(GL_TEXTURE1);
		break;
	case 2:
		glActiveTexture(GL_TEXTURE2);
		break;
	case 3:
		glActiveTexture(GL_TEXTURE3);
		break;
	default:
		return;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
}

#define INITEX( tex, path ) if(!render::texture::load_png(tex, path)) {printf("ERROR (render::texture::init): failed to load \"%s\"\n", path); return false;}
bool render::texture::init() {
	INITEX(board_texture, "assets/board.png");
	INITEX(piece_pawn_texture, "assets/pawn.png");
	return true;
}

void render::texture::bind_board(int binding) {
	bind_texture(board_texture, binding);
}

void render::texture::bind_piece_pawn(int binding) {
	bind_texture(piece_pawn_texture, binding);
}
