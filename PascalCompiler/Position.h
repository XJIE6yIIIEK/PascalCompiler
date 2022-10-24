#pragma once
#include <string>

class Position {
	public:
		unsigned int row;
		unsigned int col;

		Position(unsigned int row, unsigned int col);

		std::string to_string();
};