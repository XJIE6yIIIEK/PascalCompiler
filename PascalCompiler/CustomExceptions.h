#pragma once
#include <exception>
#include <string>
#include <memory>
#include <vector>
#include "Position.h"

class CustomException : public std::exception {
	protected:
		std::string message;
		std::string errName;
		PositionPtr pos;

	public:
		CustomException(std::string errName, std::string msg, PositionPtr pos);

		const char* what();
};

class UnexpectedToken : public CustomException {
	private:
		UnexpectedToken(std::string errName, std::string msg, PositionPtr pos);

	public:
		static UnexpectedToken CreateException(std::string tokens, PositionPtr pos);
};

class WrongName : public CustomException {
	private:
		WrongName(std::string errName, std::string msg, PositionPtr pos);

	public:
		static WrongName CreateException(PositionPtr pos);
};

class WrongSymbol : public CustomException {
	private:
		WrongSymbol(std::string errName, std::string msg, PositionPtr pos);

	public:
		static WrongSymbol CreateException(PositionPtr pos);
};

class WrongNumber : public CustomException {
	private:
		WrongNumber(std::string errName, std::string msg, PositionPtr pos);

	public:
		static WrongNumber CreateException(PositionPtr pos);
};

class EndOfProgram : public CustomException {
	private:
		EndOfProgram(std::string errName, std::string msg);

	public:
		static EndOfProgram CreateException();
};