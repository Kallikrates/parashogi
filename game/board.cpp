#include "board.hpp"

shogi::board::board() {

}

shogi::board::~board() {

}

void shogi::board::clear() {
	board_lock.lock();
	this->pieces.clear();
	board_lock.unlock();
}

bool shogi::board::add_piece(board_coords coords, piece_type type, alignment align) {
	board_lock.lock();
	for (auto const & i : this->pieces) {
		if (i.coords.x == coords.x && i.coords.y == coords.y) return false;
	}
	pieces.push_back({std::shared_ptr<shogi::piece>{new shogi::piece(type, align)}, coords});
	board_lock.unlock();
	return true;
}

std::vector<shogi::piece_placement> const & shogi::board::get_pieces() const {
	return pieces;
}

void shogi::board::lock_board() {
	board_lock.lock();
}

void shogi::board::unlock_board() {
	board_lock.unlock();
}
