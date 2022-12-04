#include "SyntaxAnalizer.h"
#include "CustomExceptions.h"

Syntax::Syntax(std::unique_ptr<Tokenizer> tokenizer) {
	this->tokenizer = std::move(tokenizer);
}

Syntax::~Syntax() { }

void Syntax::Compile() {
	GetNext();

	program();
}

void Syntax::GetNext() {
	curToken = tokenizer->GetNextToken();
}

TokenKeywords* Syntax::GetKeywordToken() {
	return dynamic_cast<TokenKeywords*>(curToken.get());
}

TokenConst* Syntax::GetConstToken() {
	return dynamic_cast<TokenConst*>(curToken.get());
}

template<typename ConstType>
TokenTypeConst<ConstType>* Syntax::GetTypeConstToken(TokenConst* tokenConst) {
	return dynamic_cast<TokenTypeConst<ConstType>*>(tokenConst);
}

bool Syntax::Accept(KeywordsType kwType, bool next = true, bool throwErr = true) {
	TokenKeywords* token = GetKeywordToken();

	if (token == nullptr) {

		return false;
	}

	if (token->kwType == kwType) {
		if (next) {
			GetNext();
		}

		return true;
	}

	
	if (throwErr) {
		std::string tokens = "a";
		throw UnexpectedToken::CreateException(tokens, std::move(curToken->pos));
	} else {
		return false;
	}
}

bool Syntax::Accept(TokenTypeEnum tokenType, bool next = true, bool throwErr = true) {
	if (curToken == nullptr) {
		//Добавить выкидывание ошибок
		return false;
	}

	if (curToken->type == tokenType) {
		if (next) {
			GetNext();
		}

		return true;
	}

	//Добавить выкидывание ошибок
	if (throwErr) {
		std::string tokens = "a";
		throw UnexpectedToken::CreateException(tokens, std::move(curToken->pos));
	} else {
		return false;
	}
}

bool Syntax::Accept(TokenConstType constType, bool next = true, bool throwErr = true) {
	TokenConst* tokenConst = GetConstToken();

	if (tokenConst == nullptr) {
		//Добавить выкидывание ошибок
		return false;
	}

	if (tokenConst->constType == constType) {
		if (next) {
			GetNext();
		}

		return true;
	}

	//Добавить выкидывание ошибок
	if (throwErr) {
		std::string tokens = "a";
		throw UnexpectedToken::CreateException(tokens, std::move(curToken->pos));
	} else {
		return false;
	}
}

void Syntax::program() {
	Accept(KeywordsType::Program);
	Accept(TokenTypeEnum::Ident);
	Accept(KeywordsType::Semicolon);
	block();
	Accept(KeywordsType::Dot);
}

void Syntax::block() {
	defineBlock();
	opBlock();
}

void Syntax::defineBlock() {	
	constBlock();
	varBlock();
	opBlock();
	typeBlock();
}

void Syntax::constBlock() {
	if (!Accept(KeywordsType::Const, false, false)) {
		return;
	}

	GetNext();

	do {
		constDeclaration();
		Accept(KeywordsType::Semicolon);
	} while (Accept(TokenTypeEnum::Ident, false, false));
}

void Syntax::constDeclaration() {
	Accept(TokenTypeEnum::Ident);
	Accept(KeywordsType::Equal);
	constant();
}

void Syntax::constant() {
	if (Accept(KeywordsType::Quote, false, false)) {
		stringConst();
		return;
	}

	if (signStart()) {
		GetNext();

		if (Accept(TokenConstType::Float, false, false) ||
			Accept(TokenConstType::Integer, false, false) ||
			Accept(TokenTypeEnum::Ident, false, false)) {
			GetNext();
			return;
		} else {
			std::string tokens = "Float, Integer, Ident";
			throw UnexpectedToken::CreateException(tokens, std::move(curToken->pos));
		}
	}

	if (Accept(TokenConstType::Float, false, false) ||
		Accept(TokenConstType::Integer, false, false) ||
		Accept(TokenTypeEnum::Ident, false, false)) {
		GetNext();
		return;
	}
}

void Syntax::stringConst() {
	Accept(KeywordsType::Quote);
	Accept(TokenConstType::String);
	Accept(KeywordsType::Quote);
}

void Syntax::varBlock() {
	if (!Accept(KeywordsType::Const, false, false)) {
		return;
	}

	GetNext();

	do {
		similarVarSection();
		Accept(KeywordsType::Semicolon);
	} while (Accept(TokenTypeEnum::Ident, false, false));
}

void Syntax::similarVarSection() {
	Accept(TokenTypeEnum::Ident);

	while (Accept(KeywordsType::Comma, false, false)) {
		GetNext();
		Accept(TokenTypeEnum::Ident);
	}

	Accept(KeywordsType::DoubleDot);
	type();
}

void Syntax::type() {
	if (constantStart()) {
		GetNext();
	} else {
		std::string tokens = "bool, char, float, int";
		throw UnexpectedToken::CreateException(tokens, std::move(curToken->pos));
	}
}

void Syntax::typeBlock() {
	
}

void Syntax::opBlock() {
	compositeOperator();
}

void Syntax::compositeOperator() {
	Accept(KeywordsType::Begin);

	operatorBlock();

	while (Accept(KeywordsType::Semicolon, false, false)) {
		GetNext();
		operatorBlock();
	}

	Accept(KeywordsType::End);
}

void Syntax::operatorBlock() {
	unmarkedOperator();
}

void Syntax::unmarkedOperator() {
	if (Accept(TokenTypeEnum::Ident, false, false)) {
		GetNext();

		simpleOperator();
	} else if (complexOperatorStart()) {
		complexOperator();
	}
	
}

void Syntax::simpleOperator() {
	assingmentOperator();
}

void Syntax::assingmentOperator() {
	Accept(TokenTypeEnum::Ident);
	Accept(KeywordsType::Define);
	expression();
}

void Syntax::expression() {
	simpleExpression();

	if (boolOpStart()) {
		GetNext();

		simpleExpression();
	}
}

void Syntax::simpleExpression() {
	if (signStart()) {
		GetNext();

		term();

		while (additiveOpStart()) {
			GetNext();

			term();
		}
	} else {
		std::string tokens = "+, -";
		throw UnexpectedToken::CreateException(tokens, std::move(curToken->pos));
	}
}

void Syntax::term() {
	factor();

	while (multiplicativeOpStart()) {
		GetNext();

		factor();
	}
}

void Syntax::factor() {
	if (Accept(TokenTypeEnum::Ident, false, false)) {
		var();
	} else if (constantStart()) {
		GetNext();
	} else if (signStart()) {
		expression();

		while (signStart()) {
			GetNext();

			expression();
		}
	} else if (Accept(KeywordsType::Not, false, false)) {
		GetNext();

		factor();
	} else {
		//Кинуть ошибку
	}
}

void Syntax::var() {
	Accept(TokenTypeEnum::Ident);

	while (Accept(KeywordsType::OpenSquare, false, false)) {
		GetNext();

		expression();

		while (Accept(KeywordsType::Comma, false, false)) {
			GetNext();

			expression();
		}

		Accept(KeywordsType::CloseSquare);
	}
}

void Syntax::complexOperator() {
	if (Accept(KeywordsType::Begin, false, false)) {
		compositeOperator();
	} else if (Accept(KeywordsType::If, false, false)) {
		ifBlock();
	} else if (Accept(KeywordsType::While, false, false)) {
		whileBlock();
	}
}

void Syntax::ifBlock() {
	Accept(KeywordsType::If);
	expression();
	Accept(KeywordsType::Then);
	operatorBlock();

	if (Accept(KeywordsType::Else, false, false)) {
		GetNext();

		operatorBlock();
	}
}

void Syntax::whileBlock() {
	Accept(KeywordsType::While);
	expression();
	Accept(KeywordsType::Do);
	operatorBlock();
}

bool Syntax::boolOpStart() {
	return Accept(KeywordsType::Equal, false, false) ||
		Accept(KeywordsType::NotEqual, false, false) ||
		Accept(KeywordsType::More, false, false) ||
		Accept(KeywordsType::MoreEqual, false, false) ||
		Accept(KeywordsType::Less, false, false) ||
		Accept(KeywordsType::LessEqual, false, false);
}

bool Syntax::signStart() {
	return Accept(KeywordsType::Plus, false, false) || 
		Accept(KeywordsType::Minus, false, false);
}

bool Syntax::additiveOpStart() {
	return Accept(KeywordsType::Plus, false, false) || 
		Accept(KeywordsType::Minus, false, false) ||
		Accept(KeywordsType::Or, false, false);
}

bool Syntax::multiplicativeOpStart() {
	return Accept(KeywordsType::Multiply, false, false) ||
		Accept(KeywordsType::Division, false, false) ||
		Accept(KeywordsType::And, false, false) ||
		Accept(KeywordsType::Mod, false, false) ||
		Accept(KeywordsType::Div, false, false);
}

bool Syntax::constantStart() {
	return Accept(TokenTypeEnum::Constant, false, false);
}

bool Syntax::complexOperatorStart() {
	return Accept(KeywordsType::Begin, false, false) ||
		Accept(KeywordsType::While, false, false) ||
		Accept(KeywordsType::If, false, false);
}