#pragma once
#include <string>
#include <map>
#include <memory>
#include "Keywords.h"
#include "Position.h"
#include "IO.h"
#include "ErrorHandler.h"

class Token;

using TokenPtr = std::unique_ptr<Token>;

enum class TokenConstType {
	Integer,
	Bool,
	Float,
	String
};

enum class TokenTypeEnum {
	Keyword,
	Ident,
	Constant
};

const std::map<TokenConstType, std::string> tokenConstTypeMap = {{
	{TokenConstType::Integer, ">="},
	{TokenConstType::Bool, "<="},
	{TokenConstType::Float, ">"},
	{TokenConstType::String, "<"}
}};

const std::map<TokenTypeEnum, std::string> tokenTypeEnumMap = {{
	{TokenTypeEnum::Keyword, "keyword"},
	{TokenTypeEnum::Ident, "identifier"},
	{TokenTypeEnum::Constant, "constant"},
}};

class Token {
	public:
		const TokenTypeEnum type;
		PositionPtr pos;

		Token(TokenTypeEnum type, PositionPtr pos);
		virtual ~Token() = NULL;

		virtual std::string to_string() = NULL;
};

class TokenKeywords : public Token {
	public:
		const KeywordsType kwType;

		TokenKeywords(TokenTypeEnum type, PositionPtr pos, KeywordsType kwType);
		~TokenKeywords();

		virtual std::string to_string();
};

class TokenIdent : public Token {
	public:
		const std::string id;

		TokenIdent(TokenTypeEnum type, PositionPtr pos, std::string id);
		~TokenIdent();

		virtual std::string to_string();
};

class TokenConst : public Token {
	public:
		const TokenConstType constType;

		TokenConst(TokenTypeEnum type, PositionPtr pos, TokenConstType constType);
		virtual ~TokenConst() = NULL;

		virtual std::string to_string() = NULL;
};

template<typename ConstType>
class TokenTypeConst : public TokenConst {
	public:
		const ConstType val;

		TokenTypeConst(TokenTypeEnum type, PositionPtr pos, TokenConstType constType, ConstType val);
		~TokenTypeConst();

		virtual std::string to_string();
};

class Tokenizer {
	private:
		IOPtr io;
		ErrorHandlerPtr errorHandler;

	public:
		Tokenizer(IOPtr io, ErrorHandlerPtr errorHandler);
		~Tokenizer();

		TokenPtr GetNextToken();
};