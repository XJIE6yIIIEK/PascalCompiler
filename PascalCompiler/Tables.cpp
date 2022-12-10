#include "Tables.h"

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

ITableTypeElementPtr TableIdent::GetType(std::string ident, UsageEnum allowedUsage) {
	if (table.contains(ident) && table[ident]->usage == allowedUsage) {
		ITableTypeElementPtr type = table[ident]->type;

		if (type != nullptr) {
			return table[ident]->type;
		} else {
			//Кинуть ошибку
		}
	} else if (outerView == nullptr) {
		//Кинуть ошибку
		return nullptr;
	} else {
		return outerView->GetType(ident, allowedUsage);
	}
}

ITableTypeElementPtr TableIdent::GetType(std::string ident, UsageEnumVector& allowedUsage) {
	if (table.contains(ident) && std::find(allowedUsage.begin(), allowedUsage.end(), table[ident]->usage) != allowedUsage.end()) {
		ITableTypeElementPtr type = table[ident]->type;

		if (type != nullptr) {
			return table[ident]->type;
		} else {
			//Кинуть ошибку
		}
	} else if (outerView == nullptr) {
		//Кинуть ошибку
		return nullptr;
	} else {
		return outerView->GetType(ident, allowedUsage);
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

ITableTypeElement::ITableTypeElement(IdentTypeEnum type) : type(type) { }
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

ScalarTableType::ScalarTableType() : ITableTypeElement(IdentTypeEnum::Scalar) { }
ScalarTableType::~ScalarTableType() { }

EnumTableType::EnumTableType() : ITableTypeElement(IdentTypeEnum::Enum), constantList({}) { }
EnumTableType::~EnumTableType() { }

void EnumTableType::Add(std::string constant) {
	if (std::find(constantList.begin(), constantList.end(), constant) == constantList.end()) {
		constantList.push_back(constant);
	} else {
		//Кинуть ошибку
	}
}

void EnumTableType::Add(std::vector<std::string>& constants) {
	std::vector<std::string>::iterator iter = constants.begin();

	while (iter != constants.end()) {
		Add(*iter++);
	}
}

ArrayTableType::ArrayTableType(ITableTypeElementPtr elementsType, DimensionalTypeVectorPtr intervalType, int dim) :
	ITableTypeElement(IdentTypeEnum::Array), elementsType(elementsType), intervalType(intervalType), dim(dim) { }
ArrayTableType::~ArrayTableType() { }

ITableTypeElementPtr ArrayTableType::GetElementsType() {
	return elementsType;
}

ITableTypeElementPtr ArrayTableType::GetIntervalType(int dim) {
	return std::dynamic_pointer_cast<IntervalTableTypeGen>(intervalType->at(dim))->baseType;
}

IntervalTableTypeGen::IntervalTableTypeGen() : ITableTypeElement(IdentTypeEnum::Interval) {}
IntervalTableTypeGen::~IntervalTableTypeGen() {}

IntervalTableTypeGen::IntervalTableTypeGen(ITableTypeElementPtr baseType, IdentConstType constType) : 
	ITableTypeElement(IdentTypeEnum::Interval), baseType(baseType), intervalType(constType) {}

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
IntervalTableType<IntervalClass>::IntervalTableType() : IntervalTableTypeGen() { }

template<typename IntervalClass>
IntervalTableType<IntervalClass>::IntervalTableType(ITableTypeElementPtr baseType, IdentConstType constType) :
	IntervalTableTypeGen(baseType, constType) { }

template<typename IntervalClass>
IntervalTableType<IntervalClass>::~IntervalTableType() { }

template<typename IntervalClass>
ITableTypeElementPtr IntervalTableType<IntervalClass>::Create(IntervalClass minValue, ITableTypeElementPtr baseType, IdentConstType constType) {
	std::shared_ptr<IntervalTableType<IntervalClass>> interval = std::make_shared<IntervalTableType<IntervalClass>>(baseType, constType);
	interval->min = minValue;
	return std::dynamic_pointer_cast<ITableTypeElement>(interval);
}

template<typename IntervalClass>
ITableTypeElementPtr IntervalTableType<IntervalClass>::Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType) {
	TokenTypeConst<IntervalClass>* constToken = dynamic_cast<TokenTypeConst<IntervalClass>*>(token);
	return Create(constToken->val, baseType, constType);
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
template ITableTypeElementPtr IntervalTableType<std::string>::Create(std::string minValue, ITableTypeElementPtr baseType, IdentConstType constType);
template ITableTypeElementPtr IntervalTableType<int>::Create(int minValue, ITableTypeElementPtr baseType, IdentConstType constType);
template ITableTypeElementPtr IntervalTableType<bool>::Create(bool minValue, ITableTypeElementPtr baseType, IdentConstType constType);
template ITableTypeElementPtr IntervalTableType<std::string>::Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType);
template ITableTypeElementPtr IntervalTableType<int>::Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType);
template ITableTypeElementPtr IntervalTableType<bool>::Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType);
template void IntervalTableType<std::string>::AddMax(ITableTypeElementPtr intervalTT, std::string maxValue);
template void IntervalTableType<int>::AddMax(ITableTypeElementPtr intervalTT, int maxValue);
template void IntervalTableType<bool>::AddMax(ITableTypeElementPtr intervalTT, bool maxValue);
template void IntervalTableType<std::string>::AddMax(ITableTypeElementPtr intervalTT, Token* token);
template void IntervalTableType<int>::AddMax(ITableTypeElementPtr intervalTT, Token* token);
template void IntervalTableType<bool>::AddMax(ITableTypeElementPtr intervalTT, Token* token);