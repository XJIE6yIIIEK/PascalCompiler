#include "Position.h"

Position::Position(unsigned int row, unsigned int col) : row(row), col(col) { }

std::string Position::to_string() {
	return "(" + std::to_string(row) + ", " + std::to_string(col) + ")";
}