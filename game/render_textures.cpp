#include "render_textures.hpp"
#include "render_loadpng.hpp"

#include <cstdio>

static GLuint board_texture = 0;

static GLuint piece_pawn_texture = 0;
static GLuint piece_pawn_glow_texture = 0;

#define INITEX( tex, path ) if(!render::texture::load_png(tex, path)) {printf("ERROR (render::texture::init): failed to load \"%s\"\n", path); return false;}
bool render::texture::init() {
	INITEX(board_texture, "assets/board.png");
	INITEX(piece_pawn_texture, "assets/pawn.png");
	INITEX(piece_pawn_glow_texture, "assets/pawn_glow.png");
	return true;
}

void render::texture::bind_board() {
	glBindTextureUnit(0, board_texture);
	glBindTextureUnit(1, 0);
}

void render::texture::bind_piece_pawn() {
	glBindTextureUnit(0, piece_pawn_texture);
	glBindTextureUnit(1, piece_pawn_glow_texture);
}
