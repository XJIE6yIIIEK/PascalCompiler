#include "IO.h"

Lexem::Lexem(std::string word, Position pos) : word(word), pos(pos) { };

IO::IO(char* path) : epPath(path), currentPosition(-1, 0), fsm(LexerFSM()), eof(false) {
	inputStream.open(epPath);
	GetNextLine();
}

IO::~IO() {
	inputStream.close();
}

bool IO::CheckEOL() {
	return currentPosition.col == currentLine.size();
}

bool IO::GetNextLine() {
	if (inputStream.eof()) {
		return true;
	}

	do {
		std::getline(inputStream, currentLine);

		currentPosition.row += 1;
		currentPosition.col = 0;

		//currentLine.erase(remove_if(currentLine.begin(), currentLine.end(), isspace), currentLine.end());
	} while (currentLine.empty() && !inputStream.eof());

	return false;
}

void IO::SkipSpaces() {
	char currentChar = currentLine[currentPosition.col];

	while (!eof && isspace(currentChar)) {
		currentPosition.col++;

		if (CheckEOL()) {
			eof = GetNextLine();
		}

		currentChar = currentLine[currentPosition.col];
	}
}

LexemPtr IO::GetNextLexem() {
	if (eof) {
		return LexemPtr(nullptr);
	}

	std::string lexem;
	char currentChar = 0;
	StateType prevState = fsm.currentState->state;
	StateType curState = fsm.currentState->state;

	SkipSpaces();

	Position startPosition = currentPosition;

	while (!eof && ((curState >= StateType::Start && curState <= StateType::State10) || curState == StateType::State12)) {
		currentChar = currentLine[currentPosition.col];
		prevState = curState;
		curState = fsm.NextState(currentChar);

		if (curState >= StateType::State0 && curState <= StateType::State11) {
			lexem += currentChar;
			currentPosition.col++;
		} else if (curState == StateType::State12) {
			break;
		}

		if (CheckEOL()) {
			eof = GetNextLine();
		}
	}

	if (!(curState == StateType::State11 || curState == StateType::State12)) {
		fsm.Reset();
	} else if (curState == StateType::State11) {
		fsm.NextState(0);
	}

	LexemPtr lexemObj = std::make_unique<Lexem>(lexem, startPosition);

	return std::move(lexemObj);
}