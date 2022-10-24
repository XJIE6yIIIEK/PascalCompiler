#pragma once

enum class LexemType {
	Null = 0,
	Integer,
	Float,
	Operator,
	KeywordOrIdent,
	Text,
	Commentary
};