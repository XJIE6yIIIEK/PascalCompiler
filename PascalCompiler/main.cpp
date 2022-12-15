#include <string>
#include <iostream>
#include "IO.h"
#include "Lexer.h"
#include "SyntaxAnalizer.h"
#include "CustomExceptions.h"
#include "ErrorHandler.h"
#include "Generator.h"

int main(int argc, char* argv[]) {
	IOPtr io = std::make_unique<IO>(argv[1]);
	ErrorHandlerPtr errorHandler = std::make_shared<ErrorHandler>();

	std::unique_ptr<Tokenizer> tokenizer = std::make_unique<Tokenizer>(std::move(io), errorHandler);
	std::unique_ptr<CodeGenerator> codegen;

	Syntax syntax(std::move(tokenizer), errorHandler, std::move(codegen));

	try {
		syntax.Compile();
	} catch (CustomException& e) {
		std::cout << e.what() << std::endl;
	}

	std::cout << "Compilation is done!" << std::endl;
	errorHandler->PrintFinalMessage();

	return 0;
}