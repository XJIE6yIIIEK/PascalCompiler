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
		int col;
		int row;

	public:
		CustomException(std::string errName, std::string msg, Position* pos);

		std::string what();
};

class UnexpectedToken : public CustomException {
	private:
		UnexpectedToken(std::string errName, std::string msg, Position* pos);

	public:
		static UnexpectedToken CreateException(std::string tokens, Position* pos);
};

class WrongName : public CustomException {
	private:
		WrongName(std::string errName, std::string msg, Position* pos);

	public:
		static WrongName CreateException(Position* pos);
};

class WrongSymbol : public CustomException {
	private:
		WrongSymbol(std::string errName, std::string msg, Position* pos);

	public:
		static WrongSymbol CreateException(Position* pos);
};

class WrongNumber : public CustomException {
	private:
		WrongNumber(std::string errName, std::string msg, Position* pos);

	public:
		static WrongNumber CreateException(Position* pos);
};

class EndOfProgram : public CustomException {
	private:
		EndOfProgram(std::string errName, std::string msg);

	public:
		static EndOfProgram CreateException();
};

class UnexpectedKeyword : public CustomException {
	private:
		UnexpectedKeyword(std::string errName, std::string msg, Position* pos);

	public:
		static UnexpectedKeyword CreateException(std::string tokens, Position* pos);
};

class UnexpectedConstant : public CustomException {
	private:
		UnexpectedConstant(std::string errName, std::string msg, Position* pos);

	public:
		static UnexpectedConstant CreateException(std::string tokens, Position* pos);
};

class UnexpectedType : public CustomException {
	private:
		UnexpectedType(std::string errName, std::string msg, Position* pos);

	public:
		static UnexpectedType CreateException(std::string expectedType, std::string realType, Position* pos);
};

class UndefinedIdentifier : public CustomException {
	private:
		UndefinedIdentifier(std::string errName, std::string msg, Position* pos);

	public:
		static UndefinedIdentifier CreateException(std::string ident, Position* pos);
};

class ExactConstant : public CustomException {
	private:
		ExactConstant(std::string errName, std::string msg, Position* pos);

	public:
		static ExactConstant CreateException(std::string constant, Position* pos);
};

class SyntaxError : public CustomException {
	private:
		SyntaxError(std::string errName, std::string msg, Position* pos);

	public:
		static SyntaxError CreateException(std::string syntaxConstr, Position* pos);
};

class UnknownOperation : public CustomException {
	private:
		UnknownOperation(std::string errName, std::string msg, Position* pos);

	public:
		static UnknownOperation CreateException(std::string typeName, std::string op, Position* pos);
};