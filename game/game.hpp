#ifndef GAME_HPP
#define GAME_HPP

#include "board.hpp"

namespace game {
	void init();
	void reset();
	void set_board_lock(bool lock);
	std::vector<shogi::piece_placement> const & get_pieces();
}

#endif //GAME_HPP
