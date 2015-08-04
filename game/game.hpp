#ifndef GAME_HPP
#define GAME_HPP

#include "board.hpp"

namespace game {

	extern shogi::board const * const & board;
	void init();
	void reset();

}

#endif //GAME_HPP
