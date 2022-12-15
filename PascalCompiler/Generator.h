#pragma once
#include <memory>
#include <map>
#include <string>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

class CodeGenerator {
	public:
		std::unique_ptr<llvm::LLVMContext> context;
		std::unique_ptr<llvm::Module> module;
		std::unique_ptr<llvm::IRBuilder<>> builder;
		std::map<std::string, llvm::Value*> TableIdent;

		llvm::Value* IntConstant(int val);
		llvm::Value* FloatConstant(double val);
		llvm::Value* StringConstant(std::string val);
		llvm::Value* BoolConstant(bool val);

		llvm::Value* Variable(std::string ident);

		template<typename Ty>
		llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* func, llvm::StringRef ident);

		void AddToTI(std::string ident, llvm::Value* val);
		llvm::Value* GetFromTI(std::string ident);
};
