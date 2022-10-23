#pragma once

#include <string>
#include <fstream>
#include <memory>
#include "Position.h"
#include "LexerFSM.h"

class Lexem;

using LexemPtr = std::unique_ptr<Lexem>;

class Lexem {
	public:
		const std::string word;
		const Position pos;

		Lexem(std::string, Position pos);
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