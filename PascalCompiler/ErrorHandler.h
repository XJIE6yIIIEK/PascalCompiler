#pragma once
#include <iostream>

class ErrorHandler;

using ErrorHandlerPtr = std::shared_ptr<ErrorHandler>;

class ErrorHandler {
	public:
		int errorsFound = 0;

		ErrorHandler();
		~ErrorHandler();

		void PrintErrorMessage(std::string msg);
		void PrintFinalMessage();
};