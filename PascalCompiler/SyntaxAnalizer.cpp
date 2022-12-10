#include "SyntaxAnalizer.h"
#include "CustomExceptions.h"

Syntax::Syntax(std::unique_ptr<Tokenizer> tokenizer) : tokenizer(std::move(tokenizer)) { }

Syntax::~Syntax() { }

void Syntax::InitializeIdentTable() {
	ITableTypeElementPtr intType = std::make_shared<ScalarTableType>();
	ITableTypeElementPtr floatType = std::make_shared<ScalarTableType>();
	ITableTypeElementPtr charType = std::make_shared<ScalarTableType>();

	ITableTypeElementPtr boolType = std::make_shared<EnumTableType>();
	EnumPtr boolEnum = std::dynamic_pointer_cast<EnumTableType>(boolType);
	boolEnum->Add("false");
	boolEnum->Add("true");

	intType->AddCast(charType);
	intType->AddCast(floatType);

	floatType->AddCast(charType);

	boolType->AddCast(charType);

	TableIdentPtr fictiveTable = std::make_shared<TableIdent>();
	tableStack.push(fictiveTable);

	fictiveTable->Add("integer", UsageEnum::Type, intType);
	fictiveTable->Add("float", UsageEnum::Type, floatType);
	fictiveTable->Add("char", UsageEnum::Type, charType);
	fictiveTable->Add("boolean", UsageEnum::Type, boolType);
	fictiveTable->Add("false", UsageEnum::Const, intType);
	fictiveTable->Add("true", UsageEnum::Const, intType);
}

void Syntax::Compile() {
	InitializeIdentTable();

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
		throw EndOfProgram::CreateException();
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
		throw EndOfProgram::CreateException();
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
		throw EndOfProgram::CreateException();
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

ITableTypeElementPtr Syntax::AcceptTypes(ITableTypeElementPtr type1, ITableTypeElementPtr type2, bool strict = false) {
	if (type1 == type2) {
		return type1;
	} else if (type1->CheckCast(type2) && !strict) {
		return type2;
	} else if (type2->CheckCast(type1) && !strict) {
		return type1;
	} else {
		//Кинуть ошибку
		return nullptr;
	}
}

ITableTypeElementPtr Syntax::GetTypeFromFictiveView(std::string ident, UsageEnum allowedUsage) {
	TableIdentPtr table = tableStack.top();

	while (table->outerView != nullptr) {
		table = table->outerView;
	}

	return table->GetType(ident, allowedUsage );
}

ITableTypeElementPtr Syntax::GetTypeFromCurrentView(std::string ident, UsageEnum allowedUsage) {
	return tableStack.top()->GetType(ident, allowedUsage );
}

ITableTypeElementPtr Syntax::GetTypeFromCurrentView(std::string ident, UsageEnumVector& allowedUsage) {
	return tableStack.top()->GetType(ident, allowedUsage);
}

std::string Syntax::GetIdentOfCurrentToken() {
	TokenIdent* ident = dynamic_cast<TokenIdent*>(curToken.get());
	return ident->id;
}

//<Программа>
void Syntax::program() {
	Accept(KeywordsType::Program); //program
	Accept(TokenTypeEnum::Ident); //<название>
	Accept(KeywordsType::Semicolon); //;
	block(); //<блок>
	Accept(KeywordsType::Dot); //.
}

//<Блок>
void Syntax::block() {
	TableIdentPtr programTable = std::make_shared<TableIdent>(tableStack.top());
	tableStack.push(programTable);

	defineBlock(); //<Раздел объявлений>
	opBlock(); //<Раздел операторов>
}

//<Раздел объявлений>
void Syntax::defineBlock() {	
	constBlock(); //<Раздел описания констант>
	typeBlock(); //<Раздел описания типов>
	varBlock(); //<Раздел описания переменных>
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
	Accept(TokenTypeEnum::Ident, false, true);
	TableIdentElementPtr element = tableStack.top()->Add(GetIdentOfCurrentToken(), UsageEnum::Const, nullptr);

	GetNext();

	Accept(KeywordsType::Equal);
	element->type = constant();
}

ITableTypeElementPtr Syntax::constantValue() {
	UsageEnumVector allowedUsage;
	ITableTypeElementPtr type;
	bool nextSy = true;

	if (Accept(TokenConstType::Float, false, false)) {
		type = GetTypeFromFictiveView("float", UsageEnum::Type);
	} else if (Accept(TokenConstType::Integer, false, false)) {
		type = GetTypeFromFictiveView("integer", UsageEnum::Type);
	} else if (Accept(TokenConstType::Bool, false, false)) {
		type = GetTypeFromFictiveView("boolean", UsageEnum::Type);
	} else if (stringQuote()) {
		type = GetTypeFromFictiveView("char", UsageEnum::Type);
		GetNext();
		Accept(TokenConstType::String);
		Accept(KeywordsType::Quote, false, true);
	} else if (Accept(TokenTypeEnum::Ident, false, false)) {
		type = GetTypeFromCurrentView(GetIdentOfCurrentToken(), UsageEnum::Const);
	} else {
		throw UnexpectedToken::CreateException("Float, Integer, Ident", std::move(curToken->pos));
	}

	GetNext();

	return type;
}

ITableTypeElementPtr Syntax::constant() {
	if (Accept(KeywordsType::Quote, false, false)) {
		stringConst();

		return GetTypeFromFictiveView("char", UsageEnum::Type);
	}

	if (signStart()) {
		GetNext();

		return constantValue();
	}

	return constantValue();
}

void Syntax::stringConst() {
	Accept(KeywordsType::Quote);
	Accept(TokenConstType::String);
	Accept(KeywordsType::Quote);
}

void Syntax::varBlock() {
	if (!Accept(KeywordsType::Var, false, false)) {
		return;
	}

	GetNext();

	do {
		similarVarSection();
		Accept(KeywordsType::Semicolon);
	} while (Accept(TokenTypeEnum::Ident, false, false));
}

void Syntax::similarVarSection() {
	std::vector<TableIdentElementPtr> tableElements;

	Accept(TokenTypeEnum::Ident, false, true);
	tableElements.push_back(tableStack.top()->Add(GetIdentOfCurrentToken(), UsageEnum::Var, nullptr));
	GetNext();

	while (Accept(KeywordsType::Comma, false, false)) {
		GetNext();
		Accept(TokenTypeEnum::Ident, false, true);
		tableElements.push_back(tableStack.top()->Add(GetIdentOfCurrentToken(), UsageEnum::Var, nullptr));
		GetNext();
	}

	Accept(KeywordsType::DoubleDot);
	ITableTypeElementPtr type = typeDeclaration();

	std::vector<TableIdentElementPtr>::iterator iter = tableElements.begin();

	while (iter != tableElements.end()) {
		(*iter++)->type = type;
	}
}

void Syntax::type() {
	if (constantStart()) {
		GetNext();
	} else {
		throw UnexpectedToken::CreateException("bool, char, float, int", std::move(curToken->pos));
	}
}

void Syntax::typeBlock() {
	if (!Accept(KeywordsType::Type, false, false)) {
		return;
	}

	GetNext();

	do {
		typeDefine();
		Accept(KeywordsType::Semicolon);
	} while (Accept(TokenTypeEnum::Ident, false, false));
}

void Syntax::typeDefine() {
	Accept(TokenTypeEnum::Ident, false, true);
	TableIdentElementPtr element = tableStack.top()->Add(GetIdentOfCurrentToken(), UsageEnum::Type, nullptr);

	GetNext();
	Accept(KeywordsType::Equal);

	element->type = typeDeclaration();
}

ITableTypeElementPtr Syntax::typeDeclaration() {
	if (Accept(KeywordsType::Array, false, false)) {
		return regularTypeDeclaration();
	} else {
		return simpleTypeDeclaration();
	}
}

ITableTypeElementPtr Syntax::simpleTypeDeclaration() {
	if (Accept(KeywordsType::OpenBracket, false, false)) {
		return enumType();
	} else if (Accept(TokenTypeEnum::Ident, false, false)) {
		if (tableStack.top()->InView(GetIdentOfCurrentToken(), UsageEnum::Type)) {
			return typeName();
		} else if (tableStack.top()->InView(GetIdentOfCurrentToken(), UsageEnum::Const)) {
			return intervalType();
		}
	} else if (constantStart() || stringQuote()) {
		return intervalType();
	} else {
		//Кинуть ошибку
	}
}

ITableTypeElementPtr Syntax::intervalType() {
	ITableTypeElementPtr startIntervalTT;
	ITableTypeElementPtr baseType;

	if (!(constantStart() || stringQuote() || Accept(TokenTypeEnum::Ident, false, false))) {
		//Кинуть ошибку
		return nullptr;
	}

	if (Accept(TokenTypeEnum::Ident, false, false)) {
		baseType = GetTypeFromCurrentView(GetIdentOfCurrentToken(), UsageEnum::Const);
		startIntervalTT = IntervalTableType<std::string>::Create(
			GetIdentOfCurrentToken(),
			baseType,
			IdentConstType::EnumChar
		);
	} else if (constantStart() || stringQuote()) {
		IdentConstType type;

		if (stringQuote()) {
			GetNext();
			Accept(TokenConstType::String, false, true);

			baseType = GetTypeFromFictiveView("char", UsageEnum::Type);
			type = IdentConstType::Char;
			startIntervalTT = IntervalTableType<std::string>::Create(curToken.get(), baseType, type);

			GetNext();
			Accept(KeywordsType::Quote, false, true);
		} else if (Accept(TokenConstType::Bool, false, false)) {
			baseType = GetTypeFromFictiveView("boolean", UsageEnum::Type);
			type = IdentConstType::Bool;
			startIntervalTT = IntervalTableType<bool>::Create(curToken.get(), baseType, type);
		} else if (Accept(TokenConstType::Integer, false, false)) {
			baseType = GetTypeFromFictiveView("integer", UsageEnum::Type);
			type = IdentConstType::Int;
			startIntervalTT = IntervalTableType<int>::Create(curToken.get(), baseType, type);
		} else {
			//Кинуть ошибку
		}
	} else {
		//Кинуть ошибку
	}

	GetNext();

	Accept(KeywordsType::TwoDots);

	if (!(constantStart() || stringQuote() || Accept(TokenTypeEnum::Ident, false, false))) {
		//Кинуть ошибку
		return nullptr;
	}

	ITableTypeElementPtr endIntervalTT;

	if (Accept(TokenTypeEnum::Ident, false, false)) {
		endIntervalTT = GetTypeFromCurrentView(GetIdentOfCurrentToken(), UsageEnum::Const);
	} else if (constantStart() || stringQuote()) {
		if (stringQuote()) {
			endIntervalTT = GetTypeFromFictiveView("char", UsageEnum::Type);
		} else if (Accept(TokenConstType::Bool, false, false)) {
			endIntervalTT = GetTypeFromFictiveView("boolean", UsageEnum::Type);
		} else if (Accept(TokenConstType::Integer, false, false)) {
			endIntervalTT = GetTypeFromFictiveView("integer", UsageEnum::Type);
		} else {
			//Кинуть ошибку
		}
	} else {
		//Кинуть ошибку
	}

	if (startIntervalTT != endIntervalTT) {
		//Кинуть ошибку
	}

	if (Accept(TokenTypeEnum::Ident, false, false)) {
		IntervalTableType<std::string>::AddMax(startIntervalTT, GetIdentOfCurrentToken());
	} else if (constantStart() || stringQuote()) {
		if (stringQuote()) {
			GetNext();
			Accept(TokenConstType::String, false, true);

			IntervalTableType<std::string>::AddMax(startIntervalTT, curToken.get());
			
			GetNext();
			Accept(KeywordsType::Quote, false, true);
		} else if (Accept(TokenConstType::Bool, false, false)) {
			IntervalTableType<bool> ::AddMax(startIntervalTT, curToken.get());
		} else {
			IntervalTableType<int>::AddMax(startIntervalTT, curToken.get());
		}
	}

	GetNext();

	return startIntervalTT;
}

ITableTypeElementPtr Syntax::typeName() {
	ITableTypeElementPtr type = GetTypeFromCurrentView(GetIdentOfCurrentToken(), UsageEnum::Type);

	GetNext();

	return type;
}

ITableTypeElementPtr Syntax::enumType() {
	GetNext();

	std::vector<std::string> idents;
	ITableTypeElementPtr enumTT = std::make_shared<EnumTableType>();
	EnumPtr enumType = std::dynamic_pointer_cast<EnumTableType>(enumTT);


	Accept(TokenTypeEnum::Ident, false, true);
	idents.push_back(GetIdentOfCurrentToken());
	tableStack.top()->Add(GetIdentOfCurrentToken(), UsageEnum::Const, enumTT);
	GetNext();

	while (Accept(KeywordsType::Comma, false, false)) {
		GetNext();

		Accept(TokenTypeEnum::Ident, false, true);
		idents.push_back(GetIdentOfCurrentToken());
		tableStack.top()->Add(GetIdentOfCurrentToken(), UsageEnum::Const, enumTT);
		GetNext();
	}

	enumType->Add(idents);

	Accept(KeywordsType::CloseBracket);

	return enumTT;
}

ITableTypeElementPtr Syntax::regularTypeDeclaration() {
	GetNext();
	Accept(KeywordsType::OpenSquare);

	int dim = 1;
	DimensionalTypeVectorPtr dims = std::make_shared<DimensionalTypeVector>();

	ITableTypeElementPtr genDim = simpleTypeDeclaration();
	dims->push_back(genDim);

	while (Accept(KeywordsType::Comma, false, false)) {
		GetNext();

		ITableTypeElementPtr dimType = simpleTypeDeclaration();
		dims->push_back(dimType);

		dim++;
	}

	Accept(KeywordsType::CloseSquare);
	Accept(KeywordsType::Of);

	ITableTypeElementPtr elementsType = componentType();
	
	return std::make_shared<ArrayTableType>(elementsType, dims, dim);
}

ITableTypeElementPtr Syntax::componentType() {
	return typeDeclaration();
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
		simpleOperator();
	} else if (complexOperatorStart()) {
		complexOperator();
	}
	
}

void Syntax::simpleOperator() {
	assingmentOperator();
}

void Syntax::assingmentOperator() {
	Accept(TokenTypeEnum::Ident, false, true);

	ITableTypeElementPtr identType = GetTypeFromCurrentView(GetIdentOfCurrentToken(), UsageEnum::Var);

	GetNext();

	Accept(KeywordsType::Define);

	ITableTypeElementPtr operType = expression();

	AcceptTypes(identType, operType);
}

ITableTypeElementPtr Syntax::expression() {
	ITableTypeElementPtr lValue = nullptr;
	ITableTypeElementPtr rValue = nullptr;

	lValue = simpleExpression();

	if (boolOpStart()) {
		GetNext();

		rValue = simpleExpression();
	}

	if (rValue != nullptr) {
		//Accept(lValue, rValue);

		return GetTypeFromFictiveView("boolean", UsageEnum::Type);
	} else {
		return lValue;
	}
}

ITableTypeElementPtr Syntax::simpleExpression() {
	ITableTypeElementPtr lValue;
	ITableTypeElementPtr rValue;

	if (signStart()) {
		GetNext();
	}

	lValue = term();

	while (additiveOpStart()) {
		GetNext();

		rValue = term();
	}

	if (rValue != nullptr) {
		return AcceptTypes(lValue, rValue);
	} else {
		return lValue;
	}
}

ITableTypeElementPtr Syntax::term() {
	ITableTypeElementPtr lValue;
	ITableTypeElementPtr rValue;

	lValue = factor();

	while (multiplicativeOpStart()) {
		GetNext();

		rValue = factor();
	}

	if (rValue != nullptr) {
		return AcceptTypes(lValue, rValue);
	} else {
		return lValue;
	}
}

ITableTypeElementPtr Syntax::factor() {
	ITableTypeElementPtr type;

	if (Accept(TokenTypeEnum::Ident, false, false)) {
		type = var();
	} else if (constantStart() || stringQuote()) {
		type = constant();
	} else if (Accept(KeywordsType::OpenBracket, false, false)) {
		GetNext();

		type = expression();

		Accept(KeywordsType::CloseBracket);
	} else if (Accept(KeywordsType::Not, false, false)) {
		GetNext();

		type = factor();
	} else {
		//Кинуть ошибку
	}

	return type;
}

ITableTypeElementPtr Syntax::var() {
	std::vector<UsageEnum> allowedUsage = { UsageEnum::Var, UsageEnum::Const };
	ITableTypeElementPtr varType = GetTypeFromCurrentView(GetIdentOfCurrentToken(), allowedUsage);

	GetNext();

	if (varType->type == IdentTypeEnum::Array) {
		return indexedVar();
	} else {
		return varType;
	}
}

ITableTypeElementPtr Syntax::indexedVar() {
	ITableTypeElementPtr varType = GetTypeFromCurrentView(GetIdentOfCurrentToken(), UsageEnum::Var);
	ITableTypeElementPtr arrayElementType = varType;

	while (Accept(KeywordsType::OpenSquare, false, false)) {
		ArrayPtr arr = std::dynamic_pointer_cast<ArrayTableType>(arrayElementType);
		arrayElementType = arr->elementsType;

		int dim = 0;

		GetNext();

		ITableTypeElementPtr indexType = expression();

		AcceptTypes(indexType, arr->GetIntervalType(dim), true);

		while (Accept(KeywordsType::Comma, false, false)) {
			dim++;

			GetNext();

			indexType = expression();

			AcceptTypes(indexType, arr->GetIntervalType(dim), true);
		}

		Accept(KeywordsType::CloseSquare);
	}

	return arrayElementType;
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

	ITableTypeElementPtr exprType = expression();
	AcceptTypes(exprType, GetTypeFromFictiveView("boolean", UsageEnum::Type));	

	Accept(KeywordsType::Then);
	operatorBlock();

	if (Accept(KeywordsType::Else, false, false)) {
		GetNext();

		operatorBlock();
	}
}

void Syntax::whileBlock() {
	Accept(KeywordsType::While);
	
	ITableTypeElementPtr exprType = expression();
	AcceptTypes(exprType, GetTypeFromFictiveView("boolean", UsageEnum::Type));

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

bool Syntax::stringQuote() {
	return Accept(KeywordsType::Quote, false, false);
}

bool Syntax::complexOperatorStart() {
	return Accept(KeywordsType::Begin, false, false) ||
		Accept(KeywordsType::While, false, false) ||
		Accept(KeywordsType::If, false, false);
}