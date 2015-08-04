#include "game.hpp"

shogi::board * board_actual = nullptr;
shogi::board const * const & game::board = board_actual;

void game::init() {
	board_actual = new shogi::board();
}

void game::reset() {
	board_actual->clear();
}
