#include <string>
#include <iostream>
#include "IO.h"
#include "Lexer.h"
#include "SyntaxAnalizer.h"
#include "CustomExceptions.h"

int main(int argc, char* argv[]) {
	IOPtr io = std::make_unique<IO>(argv[1]);
	std::unique_ptr<Tokenizer> tokenizer = std::make_unique<Tokenizer>(std::move(io));
	Syntax syntax(std::move(tokenizer));

	try {
		syntax.Compile();
	} catch (UnexpectedToken& e) {
		std::cout << e.what() << std::endl;
	}

	/*IOPtr io = std::make_unique<IO>(argv[1]);
	Tokenizer tokenizer(std::move(io));

	for (TokenPtr token = tokenizer.GetNextToken(); token; token = tokenizer.GetNextToken()) {
		switch (token->type) {
			case TokenTypeEnum::Keyword: {
				std::cout << ">>> Keyword " << token->to_string() << std::endl;
			} break;

			case TokenTypeEnum::Ident: {
				std::cout << ">>> Identificator " << token->to_string() << std::endl;
			} break;

			case TokenTypeEnum::Constant: {
				std::cout << ">>> Constant " << token->to_string() << std::endl;
			} break;
		}
	}*/


	return 0;
}