#pragma once
#include <map>
#include <vector>
#include <memory>
#include <vector>
#include <tuple>
#include "LexemType.h"

class StateNode;

using StateNodePtr = std::shared_ptr<StateNode>;
using StateNodeWeak = std::weak_ptr<StateNode>;
using StateNodeSharedVector = std::vector<StateNodePtr>;
using TupleVector = std::vector<std::tuple<char, char>>;
using TupleVectorPtr = std::unique_ptr<TupleVector>;
using StateCodesVector = std::vector<TupleVectorPtr>;

enum class StateType {
	Error = -2,
	Terminal = -1,
	Start = 0, 
	State0 = 1, 
	State1 = 2, 
	State2 = 3, 
	State3 = 4, 
	State4 = 5, 
	State5 = 6, 
	State6 = 7, 
	State7 = 8, 
	State8 = 9, 
	State9 = 10, 
	State10 = 11,
	State11 = 12,
	State12 = 13,
	State13 = 14,
	State14 = 15,
	State15 = 16
};

class StateNode {
	public:
		const StateType state;
		const LexemType type;

		std::vector<StateNodeWeak> nextStates;
		StateCodesVector nextStateCodes;


		StateNode(StateType state, LexemType type);
};

class LexerFSM {
	private:
		StateNodeSharedVector statesVector;

		void AddTransition(StateNodePtr from, StateNodePtr to, TupleVectorPtr charVector);

	public:
		StateNodePtr startState;
		StateNodePtr terminalState;
		StateNodePtr currentState;

		LexerFSM();
		~LexerFSM();

		StateType NextState(char ch);
		void Reset();
};