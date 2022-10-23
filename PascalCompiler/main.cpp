#include <string>
#include <iostream>
#include "IO.h"

int main(int argc, char* argv[]) {
	IO io(argv[1]);

	for (LexemPtr lexem = io.GetNextLexem(); lexem; lexem = io.GetNextLexem()) {
		std::cout << lexem->word << std::endl;
	}

	return 0;
}