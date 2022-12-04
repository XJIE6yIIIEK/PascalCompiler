#pragma once
#include <memory>
#include "Lexer.h"
#include "Keywords.h"

class Syntax {
	private:
		std::unique_ptr<Tokenizer> tokenizer;
		std::unique_ptr<Token> curToken;

		bool Accept(KeywordsType kwType, bool next, bool throwErr);
		bool Accept(TokenTypeEnum tokenType, bool next, bool throwErr);
		bool Accept(TokenConstType constType, bool next, bool throwErr);

		void GetNext();
		TokenKeywords* GetKeywordToken();
		TokenConst* GetConstToken();

		template<typename ConstType>
		TokenTypeConst<ConstType>* GetTypeConstToken(TokenConst* tokenConst);

		void program();
		void block();
		void constBlock();
		void constDeclaration();
		void constant();
		void varBlock();
		void opBlock();
		void typeBlock();
		void defineBlock();
		void stringConst();
		void similarVarSection();
		void type();
		void compositeOperator();
		void operatorBlock();
		void unmarkedOperator();
		void simpleOperator();
		void complexOperator();
		void assingmentOperator();
		void expression();
		void simpleExpression();
		void term();
		void factor();
		void ifBlock();
		void whileBlock();
		void var();

		bool boolOpStart();
		bool additiveOpStart();
		bool signStart();
		bool multiplicativeOpStart();
		bool constantStart();
		bool complexOperatorStart();

	public:
		Syntax(std::unique_ptr<Tokenizer> tokenizer);
		~Syntax();

		void Compile();
};
