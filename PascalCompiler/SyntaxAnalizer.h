#pragma once
#include <memory>
#include <stack>
#include <tuple>
#include "Lexer.h"
#include "Keywords.h"
#include "Tables.h"
#include "Generator.h"

using TTTuple = std::tuple<ITableTypeElementPtr, llvm::Value*>;
using TTTuplePtr = std::unique_ptr<TTTuple>;

class Syntax {
	private:
		std::unique_ptr<CodeGenerator> codegen;
		std::unique_ptr<Tokenizer> tokenizer;
		ErrorHandlerPtr errorHandler;
		std::unique_ptr<Token> curToken;

		std::stack<TableIdentPtr> tableStack;

		
		void InitializeIdentTable();
		ITableTypeElementPtr GetTypeFromFictiveView(std::string ident, UsageEnum allowedUsage);
		ITableTypeElementPtr GetTypeFromCurrentView(std::string ident, UsageEnum allowedUsage);
		ITableTypeElementPtr GetTypeFromCurrentView(std::string ident, UsageEnumVector& allowedUsage);
		TableIdentElementPtr GetRecordFromCurrentView(std::string ident, UsageEnumVector& allowedUsage);
		TableIdentElementPtr GetRecordFromCurrentView(std::string ident, UsageEnum allowedUsage);
		std::string GetIdentOfCurrentToken();

		bool Accept(KeywordsType kwType, bool next, bool throwErr);
		bool Accept(TokenTypeEnum tokenType, bool next, bool throwErr);
		bool Accept(TokenConstType constType, bool next, bool throwErr);
		ITableTypeElementPtr AcceptTypes(ITableTypeElementPtr type1, ITableTypeElementPtr type2, bool strict, bool assingment);
		ITableTypeElementPtr AcceptOperation(ITableTypeElementPtr type1, ITableTypeElementPtr type2, KeywordsType operationType, PositionPtr pos);
		ITableTypeElementPtr AcceptUnarOperation(ITableTypeElementPtr type1, KeywordsType operationType, PositionPtr pos);

		void GetNext();
		TokenKeywords* GetKeywordToken();
		TokenConst* GetConstToken();
		void SkipTo(std::vector<KeywordsType>& kws, bool skipSemicolon);

		template<typename ConstType>
		TokenTypeConst<ConstType>* GetTypeConstToken(TokenConst* tokenConst);

		void program();
		void block();
		void constBlock();
		void constDeclaration();
		TTTuple constant();
		ITableTypeElementPtr constantValue();
		void varBlock();
		void opBlock();
		void typeBlock();
		void defineBlock();
		TTTuple stringConst();
		void similarVarSection();
		void typeDefine();
		ITableTypeElementPtr typeDeclaration();
		ITableTypeElementPtr simpleTypeDeclaration();
		ITableTypeElementPtr regularTypeDeclaration();
		ITableTypeElementPtr componentType();
		ITableTypeElementPtr enumType();
		ITableTypeElementPtr intervalType();
		ITableTypeElementPtr typeName();
		void compositeOperator();
		void operatorBlock();
		void unmarkedOperator();
		void simpleOperator();
		void complexOperator();
		void assingmentOperator();
		ITableTypeElementPtr expression();
		ITableTypeElementPtr simpleExpression();
		ITableTypeElementPtr term();
		ITableTypeElementPtr factor();
		void ifBlock();
		void whileBlock();
		ITableTypeElementPtr var();
		ITableTypeElementPtr indexedVar();
		void complexOpBlock();

		bool relOpStart();
		bool additiveOpStart();
		bool signStart();
		bool multiplicativeOpStart();
		bool constantStart();
		bool complexOperatorStart();
		bool stringQuote();

	public:
		Syntax(std::unique_ptr<Tokenizer> tokenizer, ErrorHandlerPtr errorHandler, std::unique_ptr<CodeGenerator> codegen);
		~Syntax();

		void Compile();
};
