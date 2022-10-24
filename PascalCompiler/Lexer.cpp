#include "Lexer.h"

Tokenizer::Tokenizer(IOPtr io) : io(std::move(io)) {}

Tokenizer::~Tokenizer() {}

Token::Token(TokenType type, PositionPtr pos) : type(type), pos(std::move(pos)) { }

Token::~Token() { }

TokenKeywords::TokenKeywords(TokenType type, PositionPtr pos, KeywordsType kwType) : Token(type, std::move(pos)), kwType(kwType) { }

TokenKeywords::~TokenKeywords() {}

std::string TokenKeywords::to_string() {
	return enumToKeywords.find(kwType)->second + " at " + pos->to_string();
}

TokenIdent::TokenIdent(TokenType type, PositionPtr pos, std::string id) : Token(type, std::move(pos)), id(id) { }

TokenIdent::~TokenIdent() {}

std::string TokenIdent::to_string() {
	return id + " at " + pos->to_string();
}

template<typename ConstType>
TokenConst<ConstType>::TokenConst(TokenType type, PositionPtr pos, TokenConstType constType, ConstType val) : Token(type, std::move(pos)), constType(constType), val(val) {}

template<typename ConstType>
TokenConst<ConstType>::~TokenConst() {}

std::string TokenConst<int>::to_string() {
	return "INT " + std::to_string(val) + " at " + pos->to_string();
}

std::string TokenConst<bool>::to_string() {
	return "BOOL " + std::to_string(val) + " at " + pos->to_string();
}

std::string TokenConst<float>::to_string() {
	return "FLOAT " + std::to_string(val) + " at " + pos->to_string();
}

std::string TokenConst<std::string>::to_string() {
	return "STRING " + val + " at " + pos->to_string();
}

TokenPtr Tokenizer::GetNextToken() {
	LexemPtr lexem = io->GetNextLexem();

	if (lexem == nullptr) {
		return nullptr;
	}

	while (lexem->type == LexemType::Commentary) {
		lexem = io->GetNextLexem();

		if (lexem == nullptr) {
			return nullptr;
		}
	}

	TokenPtr token;

	switch(lexem->type){
		case LexemType::Operator: {
			token = std::make_unique<TokenKeywords>(TokenType::Keyword, std::move(lexem->pos), keywordsDict.find(lexem->word)->second);
		} break;

		case LexemType::KeywordOrIdent: {
			if (lexem->word == "true" || lexem->word == "false") {
				token = std::make_unique<TokenConst<bool>>(TokenType::Constant, std::move(lexem->pos), TokenConstType::Bool, (lexem->word == "true" ? true : false));
			} else if (keywordsDict.contains(lexem->word)) {
				token = std::make_unique<TokenKeywords>(TokenType::Keyword, std::move(lexem->pos), keywordsDict.find(lexem->word)->second);
			} else {
				token = std::make_unique<TokenIdent>(TokenType::Ident, std::move(lexem->pos), lexem->word);
			}
		} break;

		case LexemType::Text: {
			token = std::make_unique<TokenConst<std::string>>(TokenType::Constant, std::move(lexem->pos), TokenConstType::String, lexem->word);
		} break;

		case LexemType::Integer: {
			token = std::make_unique<TokenConst<int>>(TokenType::Constant, std::move(lexem->pos), TokenConstType::Integer, std::stoi(lexem->word));
		} break;

		case LexemType::Float: {
			token = std::make_unique<TokenConst<float>>(TokenType::Constant, std::move(lexem->pos), TokenConstType::Float, std::stof(lexem->word));
		} break;
	}

	return token;
}