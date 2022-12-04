#pragma once
#include <exception>
#include <string>
#include <memory>
#include <vector>
#include "Position.h"

class CustomException : public std::exception {
	protected:
		std::string message;
		PositionPtr pos;

	public:
		CustomException(std::string& msg, PositionPtr pos);

		const char* what();
};

class UnexpectedToken : public CustomException {
	private:
		UnexpectedToken(std::string& tokens, PositionPtr pos);

	public:
		static UnexpectedToken CreateException(std::string& msg, PositionPtr pos);
};