#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "Lexer.h"
#include "Position.h"
#include "Keywords.h"

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
using ITableTypeElementWPtr = std::weak_ptr<ITableTypeElement>;

using ScalarPtr = std::shared_ptr<ScalarTableType>;
using EnumPtr = std::shared_ptr<EnumTableType>;
using ArrayPtr = std::shared_ptr<ArrayTableType>;
using IntervalGenPtr = std::shared_ptr<IntervalTableTypeGen>;

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
	Char,
	EnumChar
};

class TableIdent {
	public:
		TableIdentPtr outerView;
		UOMTableIdent table;
		int currentPosition = 0;

		TableIdent(TableIdentPtr outerView = nullptr);
		~TableIdent();

		TableIdentElementPtr Add(std::string ident, UsageEnum usage, ITableTypeElementPtr type = nullptr);
		ITableTypeElementPtr GetType(std::string ident, UsageEnum allowedUsage, Position* pos);
		ITableTypeElementPtr GetType(std::string ident, UsageEnumVector& allowedUsage, Position* pos);
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

//Интерфейс для элементов таблицы типов
class ITableTypeElement {
	public:
		IdentTypeEnum type;
		std::vector<ITableTypeElementWPtr> allowedCasts;
		std::vector<KeywordsType> allowedOperations;

		std::string typeName;

		ITableTypeElement(IdentTypeEnum type, std::string typeName);
		virtual ~ITableTypeElement() = NULL;

		void AddCast(ITableTypeElementPtr type);
		void AddAllowedOperations(std::vector<KeywordsType>& operations);
		bool CheckCast(ITableTypeElementPtr type);
		bool CheckOperation(KeywordsType operation);
};

class ScalarTableType : public ITableTypeElement {
	public:
		ScalarTableType(std::string typeName);
		~ScalarTableType();
};

class ArrayTableType : public ITableTypeElement {
	public:
		ITableTypeElementPtr elementsType;
		DimensionalTypeVectorPtr intervalType;
		int dim;

		ArrayTableType(ITableTypeElementPtr elementsType, DimensionalTypeVectorPtr intervalType, int dim, std::string typeName);
		~ArrayTableType();

		ITableTypeElementPtr GetElementsType();
		ITableTypeElementPtr GetIntervalType(int dim);
};

class EnumTableType : public ITableTypeElement {
	public:
		std::vector<std::string> constantList;

		EnumTableType(std::string typeName);
		~EnumTableType();

		void Add(std::string constant, Position* pos);
		void Add(std::vector<std::string>& constants, Position* pos);
};

class IntervalTableTypeGen : public ITableTypeElement {
	public:
		ITableTypeElementPtr baseType = nullptr ;
		IdentConstType intervalType;

		IntervalTableTypeGen(std::string typeName);
		IntervalTableTypeGen(ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
		virtual ~IntervalTableTypeGen() = NULL;

		ITableTypeElementPtr GetType();
		static ITableTypeElementPtr GetBaseType(ITableTypeElementPtr intervalType);
		static bool CheckAllowedTypes(ITableTypeElementPtr intervalType, std::vector<IdentConstType> types);
};

template<typename IntervalClass>
class IntervalTableType : public IntervalTableTypeGen {
	public:
		IntervalClass max;
		IntervalClass min;

		IntervalTableType();
		IntervalTableType(ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
		~IntervalTableType();

		static void AddMax(ITableTypeElementPtr intervalTT, IntervalClass maxValue);
		static void AddMax(ITableTypeElementPtr intervalTT, Token* token);

		static ITableTypeElementPtr Create(IntervalClass minValue, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
		static ITableTypeElementPtr Create(Token* token, ITableTypeElementPtr baseType, IdentConstType constType, std::string typeName);
};
