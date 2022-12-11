#include "Tables.h"
#include "CustomExceptions.h"

TableIdent::TableIdent(TableIdentPtr outerView) : table({}), outerView(outerView) { }
TableIdent::~TableIdent() { }

TableIdentElementPtr TableIdent::Add(std::string ident, UsageEnum usage, ITableTypeElementPtr type) {
	if (table.find(ident) != table.end()) {
		//Кинуть ошибку
	}

	TableIdentElementPtr tableElement = std::make_shared<TableIdentElement>(usage, type);

	table[ident] = tableElement;

	return tableElement;
}

ITableTypeElementPtr TableIdent::GetType(std::string ident, UsageEnum allowedUsage, Position* pos) {
	if (table.contains(ident) && table[ident]->usage == allowedUsage) {
		ITableTypeElementPtr type = table[ident]->type;

		if (type != nullptr) {
			return table[ident]->type;
		} else {
			throw UndefinedIdentifier::CreateException(ident, pos);
		}
	} else if (outerView == nullptr) {
		throw UndefinedIdentifier::CreateException(ident, pos);
	} else {
		return outerView->GetType(ident, allowedUsage, pos);
	}
}

ITableTypeElementPtr TableIdent::GetType(std::string ident, UsageEnumVector& allowedUsage, Position* pos) {
	if (table.contains(ident) && std::find(allowedUsage.begin(), allowedUsage.end(), table[ident]->usage) != allowedUsage.end()) {
		ITableTypeElementPtr type = table[ident]->type;

		if (type != nullptr) {
			return table[ident]->type;
		} else {
			throw UndefinedIdentifier::CreateException(ident, pos);
		}
	} else if (outerView == nullptr) {
		throw UndefinedIdentifier::CreateException(ident, pos);
	} else {
		return outerView->GetType(ident, allowedUsage, pos);
	}
}

bool TableIdent::InView(std::string ident, UsageEnum allowedUsage) {
	if (table.contains(ident) && table[ident]->usage == allowedUsage) {
		return true;
	}

	if (outerView == nullptr) {
		return false;
	}

	return outerView->InView(ident, allowedUsage);
}

bool TableIdent::InView(std::string ident, UsageEnumVector& allowedUsage) {
	if (table.contains(ident) && std::find(allowedUsage.begin(), allowedUsage.end(), table[ident]->usage) != allowedUsage.end()) {
		return true;
	}

	if (outerView == nullptr) {
		return false;
	}

	return outerView->InView(ident, allowedUsage);
}

TableIdentElement::TableIdentElement(UsageEnum usage, ITableTypeElementPtr type) : usage(usage), type(type) {}
TableIdentElement::~TableIdentElement() { }

ITableTypeElement::ITableTypeElement(IdentTypeEnum type, std::string typeName) : type(type), typeName(typeName) {}
ITableTypeElement::~ITableTypeElement() { }

void ITableTypeElement::AddCast(ITableTypeElementPtr type) {
	allowedCasts.push_back(type);
}

bool ITableTypeElement::CheckCast(ITableTypeElementPtr type) {
	
	std::vector<ITableTypeElementWPtr>::iterator iter = allowedCasts.begin();

	while (iter != allowedCasts.end()) {
		if ((*iter++).lock() == type) {
			return true;
		}
	}

	return false;
}

void ITableTypeElement::AddAllowedOperations(std::vector<KeywordsType>& operations) {
	allowedOperations = operations;
}

bool ITableTypeElement::CheckOperation(KeywordsType operation) {
	if (std::find(allowedOperations.begin(), allowedOperations.end(), operation) != allowedOperations.end()) {
		return true;
	} else {
		return false;
	}
}

ScalarTableType::ScalarTableType(std::string typeName) : ITableTypeElement(IdentTypeEnum::Scalar, typeName) { }
ScalarTableType::~ScalarTableType() { }

EnumTableType::EnumTableType(std::string typeName) : ITableTypeElement(IdentTypeEnum::Enum, typeName), constantList({}) { }
EnumTableType::~EnumTableType() { }

void EnumTableType::Add(std::string constant, Position* pos) {
	if (std::find(constantList.begin(), constantList.end(), constant) == constantList.end()) {
		constantList.push_back(constant);
	} else {
		throw ExactConstant::CreateException(constant, pos);
	}
}

void EnumTableType::Add(std::vector<std::string>& constants, Position* pos) {
	std::vector<std::string>::iterator iter = constants.begin();

	while (iter != constants.end()) {
		Add(*iter++, pos);
	}
}

ArrayTableType::ArrayTableType(ITableTypeElementPtr elementsType, DimensionalTypeVectorPtr intervalType, int dim, std::string typeName) :
	ITableTypeElement(IdentTypeEnum::Array, typeName), elementsType(elementsType), intervalType(intervalType), dim(dim) { }
ArrayTableType::~ArrayTableType() { }

ITableTypeElementPtr ArrayTableType::GetElementsType() {
	return elementsType;
}

ITableTypeElementPtr ArrayTableType::GetIntervalType(int dim) {
	return std::dynamic_pointer_cast<IntervalTableTypeGen>(intervalType->at(dim))->baseType;
}

IntervalTableTypeGen::IntervalTableTypeGen(std::string typeName) : ITableTypeElement(IdentTypeEnum::Interval, typeName) {}
IntervalTableTypeGen::~IntervalTableTypeGen() {}

IntervalTableTypeGen::IntervalTableTypeGen(ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName) :
	ITableTypeElement(IdentTypeEnum::Interval, typeName), baseType(baseType), intervalType(constType) {}

ITableTypeElementPtr IntervalTableTypeGen::GetType() {
	return baseType;
}

ITableTypeElementPtr IntervalTableTypeGen::GetBaseType(ITableTypeElementPtr intervalType) {
	return std::dynamic_pointer_cast<IntervalTableTypeGen>(intervalType)->baseType;
}

bool IntervalTableTypeGen::CheckAllowedTypes(ITableTypeElementPtr intervalType, std::vector<IdentConstType> types) {
	IntervalGenPtr intervalGenType = std::dynamic_pointer_cast<IntervalTableTypeGen>(intervalType);

	if (std::find(types.begin(), types.end(), intervalGenType->intervalType) != types.end()) {
		return true;
	} else {
		return false;
	}
}

template<typename IntervalClass>
IntervalTableType<IntervalClass>::IntervalTableType() : IntervalTableTypeGen("") {}

template<typename IntervalClass>
IntervalTableType<IntervalClass>::IntervalTableType(ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName) :
	IntervalTableTypeGen(baseType, constType, typeName) { }

template<typename IntervalClass>
IntervalTableType<IntervalClass>::~IntervalTableType() { }

template<typename IntervalClass>
ITableTypeElementPtr IntervalTableType<IntervalClass>::Create(IntervalClass minValue, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName) {
	std::shared_ptr<IntervalTableType<IntervalClass>> interval = std::make_shared<IntervalTableType<IntervalClass>>(baseType, constType, typeName);
	interval->min = minValue;
	return std::dynamic_pointer_cast<ITableTypeElement>(interval);
}

template<typename IntervalClass>
ITableTypeElementPtr IntervalTableType<IntervalClass>::Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName) {
	TokenTypeConst<IntervalClass>* constToken = dynamic_cast<TokenTypeConst<IntervalClass>*>(token);
	return Create(constToken->val, baseType, constType, typeName);
}

template<typename IntervalClass>
void IntervalTableType<IntervalClass>::AddMax(ITableTypeElementPtr intervalTT, IntervalClass maxValue) {
	std::dynamic_pointer_cast<IntervalTableType>(intervalTT)->max = maxValue;
}

template<typename IntervalClass>
void IntervalTableType<IntervalClass>::AddMax(ITableTypeElementPtr intervalTT, Token* token) {
	TokenTypeConst<IntervalClass>* constToken = dynamic_cast<TokenTypeConst<IntervalClass>*>(token);
	AddMax(intervalTT, constToken->val);
}

template class IntervalTableType<std::string>;
template class IntervalTableType<int>;
template class IntervalTableType<bool>;
template ITableTypeElementPtr IntervalTableType<std::string>::Create(std::string minValue, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
template ITableTypeElementPtr IntervalTableType<int>::Create(int minValue, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
template ITableTypeElementPtr IntervalTableType<bool>::Create(bool minValue, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
template ITableTypeElementPtr IntervalTableType<std::string>::Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
template ITableTypeElementPtr IntervalTableType<int>::Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
template ITableTypeElementPtr IntervalTableType<bool>::Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
template void IntervalTableType<std::string>::AddMax(ITableTypeElementPtr intervalTT, std::string maxValue);
template void IntervalTableType<int>::AddMax(ITableTypeElementPtr intervalTT, int maxValue);
template void IntervalTableType<bool>::AddMax(ITableTypeElementPtr intervalTT, bool maxValue);
template void IntervalTableType<std::string>::AddMax(ITableTypeElementPtr intervalTT, Token* token);
template void IntervalTableType<int>::AddMax(ITableTypeElementPtr intervalTT, Token* token);
template void IntervalTableType<bool>::AddMax(ITableTypeElementPtr intervalTT, Token* token);