#pragma once
#include <map>
#include <vector>
#include <memory>
#include <vector>
#include <tuple>

class StateNode;

using StateNodePtr = std::shared_ptr<StateNode>;
using StateNodeWeak = std::weak_ptr<StateNode>;
using StateNodeSharedVector = std::vector<StateNodePtr>;
using TupleVector = std::vector<std::tuple<char, char>>;
using TupleVectorPtr = std::unique_ptr<TupleVector>;
using StateCodesVector = std::vector<TupleVectorPtr>;

enum StateType {
	Start = 0, // < | : | > | ; | , | ^ | [ | ] | { | } | / | + | - | . | ( | * | ) | a-z | A-Z | _ | ' | 0-9
	State0 = 1, // V
	State1 = 2, // . | V
	State2 = 3, // * | V
	State3 = 4, // ) | V
	State4 = 5, // a-z | A-Z | _ | V
	State5 = 6, // 0-9 | . | V
	State6 = 7, // 0-9 | V
	State7 = 8, // !(\, ') | \ | '
	State8 = 9, // V
	State9 = 10, // = | V
	State10 = 11, // > | = | V
	State11 = 12,
	State12 = 13,
	Terminal = -1,
	Error = -2
};

class StateNode {
	public:
		StateType state;

		std::vector<StateNodeWeak> nextStates;
		StateCodesVector nextStateCodes;


		StateNode(StateType state);
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