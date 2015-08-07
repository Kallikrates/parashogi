#ifndef BOARD_HPP
#define BOARD_HPP

#include "common.hpp"
#include "piece.hpp"

#include <memory>
#include <mutex>
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
		std::vector<piece_placement> const & get_pieces() const;
		void lock_board();
		void unlock_board();
	protected:
		std::vector<piece_placement> pieces;
		std::mutex board_lock;
	};

}

#endif //BOARD_HPP
