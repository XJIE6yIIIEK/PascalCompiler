#pragma once
#include <string>
#include <memory>

class Position;

using PositionPtr = std::unique_ptr<Position>;

class Position {
	public:
		unsigned int row;
		unsigned int col;

		Position(unsigned int row, unsigned int col);

		std::string to_string();
};