#include "CustomExceptions.h"

CustomException::CustomException(std::string& msg, PositionPtr pos) : message(msg), pos(std::move(pos)) { }

const char* CustomException::what() {
	return this->message.c_str();
}

UnexpectedToken UnexpectedToken::CreateException(std::string& tokens, PositionPtr pos) {
	Position* posRaw = pos.get();
	std::string message = "Expected " + tokens + " at (" + std::to_string(posRaw->row) + ", " + std::to_string(posRaw->col) + ")";
	return UnexpectedToken(message, std::move(pos));
}

UnexpectedToken::UnexpectedToken(std::string& msg, PositionPtr pos) : CustomException(msg, std::move(pos)) { }
