#ifndef RENDER_TEXTURES_HPP
#define RENDER_TEXTURES_HPP

#include "render_common.hpp"

namespace render {
namespace texture {
	bool init();
	void bind_board(int binding);
	void bind_piece_pawn(int binding);
}}

#endif //RENDER_TEXTURES_HPP
