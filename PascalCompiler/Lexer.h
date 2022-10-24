#pragma once
#include <string>
#include <map>
#include <memory>
#include "Keywords.h"
#include "Position.h"
#include "IO.h"

class Token;

using TokenPtr = std::unique_ptr<Token>;

enum class TokenType {
	Keyword,
	Ident,
	Constant
};

enum class TokenConstType {
	Integer,
	Bool,
	Float,
	String
};

class Token {
	public:
		const TokenType type;
		const PositionPtr pos;

		Token(TokenType type, PositionPtr pos);
		virtual ~Token() = NULL;

		virtual std::string to_string() = NULL;
};

class TokenKeywords : public Token {
	public:
		const KeywordsType kwType;

		TokenKeywords(TokenType type, PositionPtr pos, KeywordsType kwType);
		~TokenKeywords();

		virtual std::string to_string();
};

class TokenIdent : public Token {
	public:
		const std::string id;

		TokenIdent(TokenType type, PositionPtr pos, std::string id);
		~TokenIdent();

		virtual std::string to_string();
};

template<typename ConstType>
class TokenConst : public Token {
	public:
		const TokenConstType constType;
		const ConstType val;

		TokenConst(TokenType type, PositionPtr pos, TokenConstType constType, ConstType val);
		~TokenConst();

		virtual std::string to_string();
};

class Tokenizer {
	private:
		IOPtr io;

	public:
		Tokenizer(IOPtr io);
		~Tokenizer();

		TokenPtr GetNextToken();
};