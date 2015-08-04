#include "board.hpp"

shogi::board::board() {
	//for (uint8_t i = 0; i < 8; i++) pieces.push_back({std::shared_ptr<shogi::piece>{new shogi::piece(shogi::piece_type::PAWN, shogi::alignment::HOME)}, {i, 1}});
	//for (uint8_t i = 0; i < 8; i++) pieces.push_back({std::shared_ptr<shogi::piece>{new shogi::piece(shogi::piece_type::PAWN, shogi::alignment::AWAY)}, {i, 7}});
}

shogi::board::~board() {

}

void shogi::board::clear() {
	this->pieces.clear();
}

bool shogi::board::add_piece(board_coords coords, piece_type type, alignment align) {
	for (auto const & i : this->pieces) {
		if (i.coords.x == coords.x && i.coords.y == coords.y) return false;
	}
	pieces.push_back({std::shared_ptr<shogi::piece>{new shogi::piece(type, align)}, coords});
	return true;
}
