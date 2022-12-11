#include "CustomExceptions.h"

CustomException::CustomException(std::string errName, std::string msg, Position* pos) : errName(errName), message(msg) {
	if (pos == nullptr) {
		row = col = -1;
	} else {
		row = pos->row + 1;
		col = pos->col + 1;
	}
}

std::string CustomException::what() {
	std::string posStr;

	if (row != -1) {
		posStr = "Near (" + std::to_string(row) + ", " + std::to_string(col) + ")";
	}

	std::string msg = errName + ": " + posStr + ": " + message;
	return msg;
}

UnexpectedToken UnexpectedToken::CreateException(std::string tokens, Position* pos) {
	std::string message = "expected " + tokens + " token types.";
	return UnexpectedToken("UnexpectedToken", message, pos);
}

UnexpectedToken::UnexpectedToken(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) { }

WrongName WrongName::CreateException(Position* pos) {
	std::string message = "invalid symbol in var name.";
	return WrongName("WrongVarName", message, pos);
}

WrongName::WrongName(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

WrongSymbol WrongSymbol::CreateException(Position* pos) {
	std::string message = "unknown symbol.";
	return WrongSymbol("UnknownSymbol", message, pos);
}

WrongSymbol::WrongSymbol(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

WrongNumber WrongNumber::CreateException(Position* pos) {
	std::string message = "invalid symbol in number.";
	return WrongNumber("WrongNumber", message, pos);
}

WrongNumber::WrongNumber(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

EndOfProgram EndOfProgram::CreateException() {
	std::string message = "End of program.";
	return EndOfProgram("EndOfProgram", message);
}

EndOfProgram::EndOfProgram(std::string errName, std::string msg) : CustomException(errName, msg, nullptr) {}

UnexpectedKeyword UnexpectedKeyword::CreateException(std::string tokens, Position* pos) {
	std::string message = "expected " + tokens + " keywords.";
	return UnexpectedKeyword("UnexpectedKeyword", message, pos);
}

UnexpectedKeyword::UnexpectedKeyword(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

UnexpectedConstant UnexpectedConstant::CreateException(std::string tokens, Position* pos) {
	std::string message = "expected " + tokens + " constant types.";
	return UnexpectedConstant("UnexpectedConstant", message, pos);
}

UnexpectedConstant::UnexpectedConstant(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

UnexpectedType UnexpectedType::CreateException(std::string expectedType, std::string realType, Position* pos) {
	std::string message = "expected " + expectedType + " type, but comes " + realType + ".";
	return UnexpectedType("UnexpectedType", message, pos);
}

UnexpectedType::UnexpectedType(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

UndefinedIdentifier UndefinedIdentifier::CreateException(std::string ident, Position* pos) {
	std::string message = "unknown identifier " + ident + ".";
	return UndefinedIdentifier("UndknownIdentifier", message, pos);
}

UndefinedIdentifier::UndefinedIdentifier(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

ExactConstant ExactConstant::CreateException(std::string ident, Position* pos) {
	std::string message = "constant " + ident + " is already exists.";
	return ExactConstant("ExactConstant", message, pos);
}

ExactConstant::ExactConstant(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

SyntaxError SyntaxError::CreateException(std::string ident, Position* pos) {
	std::string message = "expected " + ident + ".";
	return SyntaxError("SyntaxError", message, pos);
}

SyntaxError::SyntaxError(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}

UnknownOperation UnknownOperation::CreateException(std::string typeName, std::string op, Position* pos) {
	std::string message = "unknown operation " + op + " for type " + typeName + ".";
	return UnknownOperation("UnknownOperation", message, pos);
}

UnknownOperation::UnknownOperation(std::string errName, std::string msg, Position* pos) : CustomException(errName, msg, pos) {}