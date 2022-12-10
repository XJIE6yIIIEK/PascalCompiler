#include "CustomExceptions.h"

CustomException::CustomException(std::string errName, std::string msg, PositionPtr pos) : errName(errName), message(msg), pos(std::move(pos)) { }

const char* CustomException::what() {
	std::string posStr;

	if (pos != nullptr) {
		posStr = "(" + std::to_string(pos->row) + ", " + std::to_string(pos->col) + ")";
	}

	std::string msg = errName + ": " + posStr + ": " + message;
	return msg.c_str();
}

UnexpectedToken UnexpectedToken::CreateException(std::string tokens, PositionPtr pos) {
	Position* posRaw = pos.get();
	std::string message = "expected " + tokens + " at";
	return UnexpectedToken("UnexpectedToken", message, std::move(pos));
}

UnexpectedToken::UnexpectedToken(std::string errName, std::string msg, PositionPtr pos) : CustomException(errName, msg, std::move(pos)) { }

WrongName WrongName::CreateException(PositionPtr pos) {
	Position* posRaw = pos.get();
	std::string message = "invalid symbol in var name";
	return WrongName("WrongVarName", message, std::move(pos));
}

WrongName::WrongName(std::string errName, std::string msg, PositionPtr pos) : CustomException(errName, msg, std::move(pos)) {}

WrongSymbol WrongSymbol::CreateException(PositionPtr pos) {
	Position* posRaw = pos.get();
	std::string message = "unknown symbol";
	return WrongSymbol("UnknownSymbol", message, std::move(pos));
}

WrongSymbol::WrongSymbol(std::string errName, std::string msg, PositionPtr pos) : CustomException(errName, msg, std::move(pos)) {}

WrongNumber WrongNumber::CreateException(PositionPtr pos) {
	Position* posRaw = pos.get();
	std::string message = "invalid symbol in number";
	return WrongNumber("WrongNumber", message, std::move(pos));
}

WrongNumber::WrongNumber(std::string errName, std::string msg, PositionPtr pos) : CustomException(errName, msg, std::move(pos)) {}

EndOfProgram EndOfProgram::CreateException() {
	std::string message = "End of program";
	return EndOfProgram("EndOfProgram", message);
}

EndOfProgram::EndOfProgram(std::string errName, std::string msg) : CustomException(errName, msg, nullptr) {}
