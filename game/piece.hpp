#ifndef PIECE_HPP
#define PIECE_HPP

#include "common.hpp"

namespace shogi {

	enum class piece_type : uint8_t {
		KING,
		ROOK,
		BISHOP,
		GOLD,
		SILVER,
		KNIGHT,
		LANCE,
		PAWN,

	};

	enum class alignment : uint8_t {
		HOME,
		AWAY,
	};

	class piece {
	public:
		struct pod_type {piece_type type; alignment align; uint8_t promoted;};
		piece(piece_type, alignment);
		piece_type const type;
		alignment const align;
		inline void promote();
		inline pod_type make_pod();
		inline bool is_promoted();
	protected:
		uint8_t promoted = 0;
	};

}

#endif //PIECE_HPP
