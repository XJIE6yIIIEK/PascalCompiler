#include <string>
#include <iostream>
#include "IO.h"
#include "Lexer.h"

int main(int argc, char* argv[]) {
	IOPtr io = std::make_unique<IO>(argv[1]);
	Tokenizer tokenizer(std::move(io));

	for (TokenPtr token = tokenizer.GetNextToken(); token; token = tokenizer.GetNextToken()) {
		switch (token->type) {
			case TokenType::Keyword: {
				std::cout << ">>> Keyword " << token->to_string() << std::endl;
			} break;

			case TokenType::Ident: {
				std::cout << ">>> Identificator " << token->to_string() << std::endl;
			} break;

			case TokenType::Constant: {
				std::cout << ">>> Constant " << token->to_string() << std::endl;
			} break;
		}
	}

	return 0;
}