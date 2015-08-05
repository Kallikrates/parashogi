#include "game.hpp"

shogi::board * board_actual = nullptr;
shogi::board const * const & game::board = board_actual;

void game::init() {
	board_actual = new shogi::board();
	game::reset();
}

void game::reset() {
	board_actual->clear();
	for (uint8_t i = 0; i < 9; i++) board_actual->add_piece({i, 2}, shogi::piece_type::PAWN, shogi::alignment::HOME);
	for (uint8_t i = 0; i < 9; i++) board_actual->add_piece({i, 6}, shogi::piece_type::PAWN, shogi::alignment::AWAY);
}
