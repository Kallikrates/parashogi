#include "piece.hpp"

shogi::piece::piece(piece_type type, alignment align) : align(align), type(type)  { }

void shogi::piece::promote() {
	this->promoted = 1;
}

shogi::piece::pod_type shogi::piece::make_pod() {
	return {type, align, promoted};
}

bool shogi::piece::is_promoted() {
	return promoted;
}
