#include "SyntaxAnalizer.h"
#include "CustomExceptions.h"
#include "Keywords.h"

Syntax::Syntax(std::unique_ptr<Tokenizer> tokenizer) : tokenizer(std::move(tokenizer)) { }

Syntax::~Syntax() { }

void Syntax::InitializeIdentTable() {
	ITableTypeElementPtr intType = std::make_shared<ScalarTableType>("integer");
	ITableTypeElementPtr floatType = std::make_shared<ScalarTableType>("float");
	ITableTypeElementPtr charType = std::make_shared<ScalarTableType>("char");

	ITableTypeElementPtr boolType = std::make_shared<EnumTableType>("boolean");
	EnumPtr boolEnum = std::dynamic_pointer_cast<EnumTableType>(boolType);
	boolEnum->Add("false", nullptr);
	boolEnum->Add("true", nullptr);

	intType->AddCast(charType);
	intType->AddCast(floatType);

	floatType->AddCast(charType);

	boolType->AddCast(charType);

	std::vector<KeywordsType> intOper = {
		KeywordsType::Multiply, KeywordsType::Division,
		KeywordsType::Mod, KeywordsType::Div,
		KeywordsType::Plus, KeywordsType::Minus,
		KeywordsType::Equalty, KeywordsType::Less,
		KeywordsType::LessEqual, KeywordsType::More,
		KeywordsType::MoreEqual, KeywordsType::NotEqual
	};

	std::vector<KeywordsType> floatOper = {
		KeywordsType::Multiply, KeywordsType::Division,
		KeywordsType::Plus, KeywordsType::Minus,
		KeywordsType::Equalty, KeywordsType::Less,
		KeywordsType::LessEqual, KeywordsType::More,
		KeywordsType::MoreEqual, KeywordsType::NotEqual
	};

	std::vector<KeywordsType> charOper = {
		KeywordsType::Plus,KeywordsType::Equalty, 
		KeywordsType::Less,KeywordsType::LessEqual, 
		KeywordsType::More,KeywordsType::MoreEqual, 
		KeywordsType::NotEqual
	};

	std::vector<KeywordsType> boolOper = {
		KeywordsType::Not, KeywordsType::And,
		KeywordsType::Or, KeywordsType::Equalty, 
		KeywordsType::NotEqual,

	};

	intType->AddAllowedOperations(intOper);
	floatType->AddAllowedOperations(floatOper);
	charType->AddAllowedOperations(charOper);
	boolType->AddAllowedOperations(boolOper);

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
	if (curToken == nullptr) {
		throw EndOfProgram::CreateException();
	}

	TokenKeywords* token = GetKeywordToken();

	if (token == nullptr) {
		if (throwErr) {
			throw UnexpectedKeyword::CreateException(enumToKeywords.find(kwType)->second, curToken->pos.get());
		} else {
			return false;
		}
	}

	if (token->kwType == kwType) {
		if (next) {
			GetNext();
		}

		return true;
	}

	
	if (throwErr) {
		throw UnexpectedKeyword::CreateException(enumToKeywords.find(kwType)->second, curToken->pos.get());
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

	if (throwErr) {
		throw UnexpectedToken::CreateException(tokenTypeEnumMap.find(tokenType)->second, curToken->pos.get());
	} else {
		return false;
	}
}

bool Syntax::Accept(TokenConstType constType, bool next = true, bool throwErr = true) {
	if (curToken == nullptr) {
		throw EndOfProgram::CreateException();
	}

	TokenConst* tokenConst = GetConstToken();

	if (tokenConst == nullptr) {
		if (throwErr) {
			throw UnexpectedConstant::CreateException(tokenConstTypeMap.find(constType)->second, curToken->pos.get());
		} else {
			return false;
		}
	}

	if (tokenConst->constType == constType) {
		if (next) {
			GetNext();
		}

		return true;
	}

	if (throwErr) {
		throw UnexpectedConstant::CreateException(tokenConstTypeMap.find(constType)->second, curToken->pos.get());
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
		throw UnexpectedType::CreateException(type1->typeName, type2->typeName, curToken->pos.get());
	}
}

ITableTypeElementPtr Syntax::AcceptOperation(ITableTypeElementPtr type1, ITableTypeElementPtr type2, KeywordsType operationType, PositionPtr pos) {
	ITableTypeElementPtr opType = AcceptTypes(type1, type2);

	if (opType->CheckOperation(operationType)) {
		if (operationType >= KeywordsType::Plus) {
			return opType;
		} else {
			return GetTypeFromFictiveView("boolean", UsageEnum::Type);
		}
	} else {
		throw UnknownOperation::CreateException(opType->typeName, enumToKeywords.find(operationType)->second, pos.get());
	}
}

ITableTypeElementPtr Syntax::AcceptUnarOperation(ITableTypeElementPtr type1, KeywordsType operationType, PositionPtr pos) {
	if (type1->CheckOperation(operationType)) {
		if (operationType == KeywordsType::Plus || operationType == KeywordsType::Minus) {
			return type1;
		} else {
			return GetTypeFromFictiveView("boolean", UsageEnum::Type);
		}
	} else {
		throw UnknownOperation::CreateException(type1->typeName, enumToKeywords.find(operationType)->second, pos.get());
	}
}

ITableTypeElementPtr Syntax::GetTypeFromFictiveView(std::string ident, UsageEnum allowedUsage) {
	TableIdentPtr table = tableStack.top();

	while (table->outerView != nullptr) {
		table = table->outerView;
	}

	return table->GetType(ident, allowedUsage, curToken->pos.get());
}

ITableTypeElementPtr Syntax::GetTypeFromCurrentView(std::string ident, UsageEnum allowedUsage) {
	return tableStack.top()->GetType(ident, allowedUsage, curToken->pos.get());
}

ITableTypeElementPtr Syntax::GetTypeFromCurrentView(std::string ident, UsageEnumVector& allowedUsage) {
	return tableStack.top()->GetType(ident, allowedUsage, curToken->pos.get());
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
		throw SyntaxError::CreateException("<constant identifier> or constant", curToken->pos.get());
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
		throw SyntaxError::CreateException("<enum>, <identifier> or constant", curToken->pos.get());
	}
}

ITableTypeElementPtr Syntax::intervalType() {
	ITableTypeElementPtr startIntervalTT;
	ITableTypeElementPtr baseType;

	if (!(constantStart() || stringQuote() || Accept(TokenTypeEnum::Ident, false, false))) {
		throw SyntaxError::CreateException("<enum identifier> or constant", curToken->pos.get());
	}

	if (Accept(TokenTypeEnum::Ident, false, false)) {
		baseType = GetTypeFromCurrentView(GetIdentOfCurrentToken(), UsageEnum::Const);
		startIntervalTT = IntervalTableType<std::string>::Create(
			GetIdentOfCurrentToken(),
			baseType,
			IdentConstType::EnumChar,
			GetIdentOfCurrentToken()
		);
	} else if (constantStart() || stringQuote()) {
		IdentConstType type;

		if (stringQuote()) {
			GetNext();
			Accept(TokenConstType::String, false, true);

			baseType = GetTypeFromFictiveView("char", UsageEnum::Type);
			type = IdentConstType::Char;
			startIntervalTT = IntervalTableType<std::string>::Create(curToken.get(), baseType, type, "char");

			GetNext();
			Accept(KeywordsType::Quote, false, true);
		} else if (Accept(TokenConstType::Bool, false, false)) {
			baseType = GetTypeFromFictiveView("boolean", UsageEnum::Type);
			type = IdentConstType::Bool;
			startIntervalTT = IntervalTableType<bool>::Create(curToken.get(), baseType, type, "boolean");
		} else if (Accept(TokenConstType::Integer, false, false)) {
			baseType = GetTypeFromFictiveView("integer", UsageEnum::Type);
			type = IdentConstType::Int;
			startIntervalTT = IntervalTableType<int>::Create(curToken.get(), baseType, type, "integer");
		} else {
			throw SyntaxError::CreateException("<enum identifier> or constant", curToken->pos.get());
		}
	} else {
		throw SyntaxError::CreateException("<enum identifier> or constant", curToken->pos.get());
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
			throw SyntaxError::CreateException("<enum identifier> or constant", curToken->pos.get());
		}
	} else {
		throw SyntaxError::CreateException("<enum identifier> or constant", curToken->pos.get());
	}

	AcceptTypes(startIntervalTT, endIntervalTT);

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
	ITableTypeElementPtr enumTT = std::make_shared<EnumTableType>("enumerable");
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

	enumType->Add(idents, curToken->pos.get());

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
	
	return std::make_shared<ArrayTableType>(elementsType, dims, dim, "array");
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
	ITableTypeElementPtr lValue;
	ITableTypeElementPtr rValue;
	KeywordsType op;
	PositionPtr pos;

	lValue = simpleExpression();

	if (relOpStart()) {
		op = GetKeywordToken()->kwType;
		pos = std::make_unique<Position>(curToken->pos->row, curToken->pos->col);

		GetNext();

		rValue = simpleExpression();
	}

	if (rValue != nullptr) {
		return AcceptOperation(lValue, rValue, op, std::move(pos));
	} else {
		return lValue;
	}
}

ITableTypeElementPtr Syntax::simpleExpression() {
	ITableTypeElementPtr lValue;
	ITableTypeElementPtr rValue;
	KeywordsType op;
	PositionPtr pos;

	if (signStart()) {
		GetNext();
	}

	lValue = term();

	while (additiveOpStart()) {
		op = GetKeywordToken()->kwType;
		pos = std::make_unique<Position>(curToken->pos->row, curToken->pos->col);

		GetNext();

		rValue = term();
	}

	if (rValue != nullptr) {
		return AcceptOperation(lValue, rValue, op, std::move(pos));
	} else {
		return lValue;
	}
}

ITableTypeElementPtr Syntax::term() {
	ITableTypeElementPtr lValue;
	ITableTypeElementPtr rValue;
	KeywordsType op;
	PositionPtr pos;

	lValue = factor();

	while (multiplicativeOpStart()) {
		op = GetKeywordToken()->kwType;
		pos = std::make_unique<Position>(curToken->pos->row, curToken->pos->col);

		GetNext();

		rValue = factor();
	}

	if (rValue != nullptr) {
		return AcceptOperation(lValue, rValue, op, std::move(pos));
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
		KeywordsType op = GetKeywordToken()->kwType;
		PositionPtr pos = std::make_unique<Position>(curToken->pos->row, curToken->pos->col);

		GetNext();

		type = factor();
		type = AcceptUnarOperation(type, op, std::move(pos));
	} else {
		throw SyntaxError::CreateException("<var identifier>, (, not, constant", curToken->pos.get());
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

void Syntax::complexOpBlock() {
	if (Accept(KeywordsType::Begin, false, false)) {
		opBlock();
	} else {
		operatorBlock();
	}
}

void Syntax::ifBlock() {
	Accept(KeywordsType::If);

	ITableTypeElementPtr exprType = expression();
	AcceptTypes(exprType, GetTypeFromFictiveView("boolean", UsageEnum::Type));	

	Accept(KeywordsType::Then);
	complexOpBlock();

	if (Accept(KeywordsType::Else, false, false)) {
		GetNext();

		complexOpBlock();
	}

	Accept(KeywordsType::Semicolon);
}

void Syntax::whileBlock() {
	Accept(KeywordsType::While);
	
	ITableTypeElementPtr exprType = expression();
	AcceptTypes(exprType, GetTypeFromFictiveView("boolean", UsageEnum::Type));

	Accept(KeywordsType::Do);

	complexOpBlock();

	Accept(KeywordsType::Semicolon);
}

bool Syntax::relOpStart() {
	return Accept(KeywordsType::Equalty, false, false) ||
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
