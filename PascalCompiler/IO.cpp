#include "IO.h"

Lexem::Lexem(std::string word, PositionPtr pos, LexemType type) : word(word), pos(std::move(pos)), type(type) { };

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
		return nullptr;
	}

	std::string lexem;
	char currentChar = 0;
	LexemType type = LexemType::Operator;
	StateType curState = fsm.currentState->state;

	if (curState != StateType::State7 && curState != StateType::State3) {
		SkipSpaces();
	}

	PositionPtr startPosition = std::make_unique<Position>(currentPosition.row, currentPosition.col);

	while (!eof && ((curState >= StateType::Start && curState <= StateType::State11) || (curState >= StateType::State14 && curState <= StateType::State19))) {
		bool eol = false;
		currentChar = currentLine[currentPosition.col];
		curState = fsm.NextState(currentChar);

		if (fsm.currentState->type > LexemType::Null) {
			type = fsm.currentState->type;
		} else if ((int)(curState) < -1) {
			fsm.ThrowError(std::move(startPosition));
		}

		if ((curState >= StateType::State0 && curState <= StateType::State13) || (curState >= StateType::State16 && curState <= StateType::State19)) {
			if (curState == StateType::State16 
				&& (currentPosition.col + 1 == currentLine.size() || currentLine[currentPosition.col + 1] == '.')) {
				break;
			}

			lexem += currentChar;
			currentPosition.col++;
		} else if (curState >= StateType::State14 && curState <= StateType::State15) {
			break;
		}

		if (CheckEOL()) {
			eof = GetNextLine();
			eol = true;
		}

		if (!(curState == StateType::State7 || curState == StateType::State3) && eol) {
			break;
		} else if (eol) {
			lexem += '\n';
		}
	}

	if (!(curState >= StateType::State12 && curState <= StateType::State16)) {
		fsm.Reset();
	} else if (curState >= StateType::State12 && curState <= StateType::State13) {
		fsm.NextState(0);
	}

	return std::make_unique<Lexem>(lexem, std::move(startPosition), type);
}