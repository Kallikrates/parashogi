#include "game.hpp"

static shogi::board * board_actual = nullptr;

void game::init() {
	board_actual = new shogi::board();
	game::reset();
}

void game::reset() {
	board_actual->clear();
	for (uint8_t i = 0; i < 9; i++) board_actual->add_piece({i, 2}, shogi::piece_type::PAWN, shogi::alignment::HOME);
	for (uint8_t i = 0; i < 9; i++) board_actual->add_piece({i, 6}, shogi::piece_type::PAWN, shogi::alignment::AWAY);
}

void game::set_board_lock(bool lock) {
	if (lock) board_actual->lock_board(); else board_actual->unlock_board();
}

std::vector<shogi::piece_placement> const & game::get_pieces() {
	return board_actual->get_pieces();
}
