#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "Lexer.h"

class ITableTypeElement;
class TableIdent;
class TableIdentElement;
class ScalarTableType;
class EnumTableType;
class IntervalTableTypeGen;
template<typename ConstType>
class IntervalTableType;
class ArrayTableType;
enum class UsageEnum;

//UOM => Unordered Map
using TableIdentPtr = std::shared_ptr<TableIdent>;
using TableIdentElementPtr = std::shared_ptr<TableIdentElement>;
using UOMTableIdent = std::unordered_map<std::string, TableIdentElementPtr>;
using ITableTypeElementPtr = std::shared_ptr<ITableTypeElement>;

using ScalarPtr = std::shared_ptr<ScalarTableType>;
using EnumPtr = std::shared_ptr<EnumTableType>;
using ArrayPtr = std::shared_ptr<ArrayTableType>;

using DimensionalTypeVector = std::vector<ITableTypeElementPtr>;
using DimensionalTypeVectorPtr = std::shared_ptr<std::vector<ITableTypeElementPtr>>;

using UsageEnumVector = std::vector<UsageEnum>;

enum class UsageEnum {
	Var,
	Type,
	Const
};

enum class IdentTypeEnum {
	Scalar,
	Array,
	Interval,
	Enum
};

enum class IdentConstType {
	Int,
	Bool,
	Float,
	Char
};

class TableIdent {
	public:
		TableIdentPtr outerView;
		UOMTableIdent table;
		int currentPosition = 0;

		TableIdent(TableIdentPtr outerView = nullptr);
		~TableIdent();

		TableIdentElementPtr Add(std::string ident, UsageEnum usage, ITableTypeElementPtr type = nullptr);
		ITableTypeElementPtr GetType(std::string ident, UsageEnum allowedUsage);
		ITableTypeElementPtr GetType(std::string ident, UsageEnumVector& allowedUsage);
		bool InView(std::string ident, UsageEnum allowedUsage);
		bool InView(std::string ident, UsageEnumVector& allowedUsage);
};

class TableIdentElement {
	public:
		UsageEnum usage;
		ITableTypeElementPtr type;

		TableIdentElement(UsageEnum usage, ITableTypeElementPtr type);
		~TableIdentElement();
};

class TableType {
	
};

//Интерфейс для элементов таблицы типов
class ITableTypeElement {
	public:
		IdentTypeEnum type;

		ITableTypeElement(IdentTypeEnum type);
		virtual ~ITableTypeElement() = NULL;
};

class ScalarTableType : public ITableTypeElement {
	public:
		ScalarTableType();
		~ScalarTableType();
};

class ArrayTableType : public ITableTypeElement {
	public:
		ITableTypeElementPtr elementsType;
		DimensionalTypeVectorPtr intervalType;
		int dim;

		ArrayTableType(ITableTypeElementPtr elementsType, DimensionalTypeVectorPtr intervalType, int dim);
		~ArrayTableType();

		ITableTypeElementPtr GetElementsType();
		ITableTypeElementPtr GetIntervalType(int dim);
};

class EnumTableType : public ITableTypeElement {
	public:
		std::vector<std::string> constantList;

		EnumTableType();
		~EnumTableType();

		void Add(std::string constant);
		void Add(std::vector<std::string>& constants);
};

class IntervalTableTypeGen : public ITableTypeElement {
	public:
		ITableTypeElementPtr baseType = nullptr ;
		IdentConstType intervalType;

		IntervalTableTypeGen();
		IntervalTableTypeGen(ITableTypeElementPtr baseType, IdentConstType constType);
		virtual ~IntervalTableTypeGen() = NULL;

		ITableTypeElementPtr GetBaseType();
};

template<typename IntervalClass>
class IntervalTableType : public IntervalTableTypeGen {
	public:
		IntervalClass max;
		IntervalClass min;

		IntervalTableType();
		IntervalTableType(ITableTypeElementPtr baseType, IdentConstType constType);
		~IntervalTableType();

		static void AddMax(ITableTypeElementPtr intervalTT, IntervalClass maxValue);
		static void AddMax(ITableTypeElementPtr intervalTT, Token* token);

		static ITableTypeElementPtr Create(IntervalClass minValue, ITableTypeElementPtr baseType, IdentConstType constType);
		static ITableTypeElementPtr Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType);
};
