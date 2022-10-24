#pragma once

#include <string>
#include <fstream>
#include <memory>
#include "LexemType.h"
#include "Position.h"
#include "LexerFSM.h"

class Lexem;
class IO;

using LexemPtr = std::unique_ptr<Lexem>;
using PositionPtr = std::unique_ptr<Position>;
using IOPtr = std::unique_ptr<IO>;

class Lexem {
	public:
		const std::string word;
		PositionPtr pos;
		const LexemType type;

		Lexem(std::string, PositionPtr pos, LexemType type);
};

class IO {
	private:
		const std::string epPath;
		Position currentPosition;
		std::string currentLine;
		LexerFSM fsm;
		std::ifstream inputStream;
		bool eof;

		void SkipSpaces();
		bool CheckEOL();

	public:
		IO(char* path);
		~IO();

		bool GetNextLine();
		LexemPtr GetNextLexem();
};