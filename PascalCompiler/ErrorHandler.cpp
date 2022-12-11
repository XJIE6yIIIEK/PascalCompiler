#include <string>
#include "ErrorHandler.h"


ErrorHandler::ErrorHandler() {}
ErrorHandler::~ErrorHandler() {}

void ErrorHandler::PrintErrorMessage(std::string msg) {
	std::cout << msg << std::endl;
	errorsFound++;
}

void ErrorHandler::PrintFinalMessage() {
	std::cout << "Founded " + std::to_string(errorsFound) + " errors." << std::endl;
}