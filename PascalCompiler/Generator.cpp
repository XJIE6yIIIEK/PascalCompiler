#include "Generator.h"

llvm::Value* CodeGenerator::IntConstant(int val) {
	return llvm::ConstantInt::get(*context, llvm::APInt(val, 32));
}

llvm::Value* CodeGenerator::FloatConstant(double val) {
	return llvm::ConstantFP::get(*context, llvm::APFloat(val));
}

llvm::Value* CodeGenerator::StringConstant(std::string val) {
	return (*builder).CreateGlobalStringPtr(llvm::StringRef(val)); //Возврат i8*
}

llvm::Value* CodeGenerator::BoolConstant(bool val) {
	return llvm::ConstantInt::get(*context, llvm::APInt(val, 1)); //Представляем bool, как однобитовое целое
}

llvm::Value* CodeGenerator::Variable(std::string ident) {

}

template<typename Ty>
llvm::AllocaInst* CodeGenerator::CreateEntryBlockAlloca<Ty>(llvm::Function* func, llvm::StringRef ident) {
	llvm::IRBuilder<> block(&func->getEntryBlock(), func->getEntryBlock().begin());

	llvm::Type* type;

	if (typeof(Ty) == std::string) {
		type = llvm::Type::getInt8PtrTy(*context);
	} else if (typeof(Ty) == int) {
		type = llvm::Type::getInt32Ty(*context);
	} else if (typeof(Ty) == double) {
		type = llvm::Type::getDoubleTy(*context);
	} else {
		type = llvm::Type::getInt1Ty(*context);
	}

	return block.CreateAlloca(type, nullptr, ident);
}

void CodeGenerator::AddToTI(std::string ident, llvm::Value* val) {
	TableIdent[ident] = val;
}

llvm::Value* CodeGenerator::GetFromTI(std::string ident) {
	return TableIdent[ident];
}