#ifndef BOARD_HPP
#define BOARD_HPP

#include "common.hpp"
#include "piece.hpp"

#include <memory>
#include <vector>

namespace shogi {

	struct board_coords {
		uint8_t x, y;
	};

	struct piece_placement {
		std::shared_ptr<shogi::piece> piece;
		board_coords coords;
	};

	class board {
	public:
		board();
		virtual ~board();
		void clear();
		bool add_piece(board_coords, piece_type, alignment);
	protected:
		std::vector<piece_placement> pieces;
	};

}

#endif //BOARD_HPP
