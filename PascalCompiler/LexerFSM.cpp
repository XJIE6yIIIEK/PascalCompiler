#include "LexerFSM.h"

StateNode::StateNode(StateType state) : state(state) {}

LexerFSM::LexerFSM() : startState(std::make_shared<StateNode>(StateType::Start)), terminalState(std::make_shared<StateNode>(StateType::Terminal)) {
	StateNodePtr state0 = std::make_shared<StateNode>(StateType::State0);
	StateNodePtr state1 = std::make_shared<StateNode>(StateType::State1);
	StateNodePtr state2 = std::make_shared<StateNode>(StateType::State2);
	StateNodePtr state3 = std::make_shared<StateNode>(StateType::State3);
	StateNodePtr state4 = std::make_shared<StateNode>(StateType::State4);
	StateNodePtr state5 = std::make_shared<StateNode>(StateType::State5);
	StateNodePtr state6 = std::make_shared<StateNode>(StateType::State6);
	StateNodePtr state7 = std::make_shared<StateNode>(StateType::State7);
	StateNodePtr state8 = std::make_shared<StateNode>(StateType::State8);
	StateNodePtr state9 = std::make_shared<StateNode>(StateType::State9);
	StateNodePtr state10 = std::make_shared<StateNode>(StateType::State10);
	StateNodePtr state11 = std::make_shared<StateNode>(StateType::State11);
	StateNodePtr state12 = std::make_shared<StateNode>(StateType::State12);

	statesVector.push_back(state0);
	statesVector.push_back(state1);
	statesVector.push_back(state2);
	statesVector.push_back(state3);
	statesVector.push_back(state4);
	statesVector.push_back(state5);
	statesVector.push_back(state6);
	statesVector.push_back(state7);
	statesVector.push_back(state8);
	statesVector.push_back(state9);
	statesVector.push_back(state10);
	statesVector.push_back(state11);
	statesVector.push_back(state12);

	//START INITIALIZING
	TupleVectorPtr state_Start_1 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('.', '.')
	}));
	AddTransition(startState, state1, std::move(state_Start_1));

	TupleVectorPtr state_Start_2 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('(', '(')
	}));
	AddTransition(startState, state2, std::move(state_Start_2));

	TupleVectorPtr state_Start_3 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('*', '*')
	}));
	AddTransition(startState, state3, std::move(state_Start_3));

	TupleVectorPtr state_Start_4 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('a', 'z'),
		std::make_tuple('A', 'Z'),
		std::make_tuple('_', '_')
	}));
	AddTransition(startState, state4, std::move(state_Start_4));

	TupleVectorPtr state_Start_5 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('0', '9')
	}));
	AddTransition(startState, state5, std::move(state_Start_5));

	TupleVectorPtr state_Start_11 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('\'', '\'')
	}));
	AddTransition(startState, state11, std::move(state_Start_11));

	TupleVectorPtr state_Start_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple(';', ';'),
		std::make_tuple(',', ','),
		std::make_tuple('^', '^'),
		std::make_tuple('[', '['),
		std::make_tuple(']', ']'),
		std::make_tuple('{', '{'),
		std::make_tuple('}', '}'),
		std::make_tuple('/', '/'),
		std::make_tuple('+', '+'),
		std::make_tuple('-', '-'),
		std::make_tuple('=', '='),
		std::make_tuple(')', ')')
	}));
	AddTransition(startState, state0, std::move(state_Start_0));

	TupleVectorPtr state_Start_9 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple(':', ':'),
		std::make_tuple('>', '>')
	}));
	AddTransition(startState, state9, std::move(state_Start_9));

	TupleVectorPtr state_Start_10 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('<', '<')
	}));
	AddTransition(startState, state10, std::move(state_Start_10));
	//START COMPLETE

	//STATE 1 INITIALIZING
	TupleVectorPtr state_1_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('.', '.')
	}));
	AddTransition(state1, state0, std::move(state_1_0));

	TupleVectorPtr state_1_T = nullptr;
	AddTransition(state1, terminalState, std::move(state_1_T));
	//STATE 1 COMPLETE

	//STATE 2 INITIALIZING
	TupleVectorPtr state_2_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('*', '*')
	}));
	AddTransition(state2, state0, std::move(state_2_0));

	TupleVectorPtr state_2_T = nullptr;
	AddTransition(state2, terminalState, std::move(state_2_T));
	//STATE 2 COMPLETE

	//STATE 3 INITIALIZING
	TupleVectorPtr state_3_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple(')', ')')
	}));
	AddTransition(state3, state0, std::move(state_3_0));

	TupleVectorPtr state_3_T = nullptr;
	AddTransition(state3, terminalState, std::move(state_3_T));
	//STATE 3 COMPLETE

	//STATE 4 INITIALIZING
	TupleVectorPtr state_4_4 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('a', 'z'),
		std::make_tuple('A', 'Z'),
		std::make_tuple('_', '_')
	}));
	AddTransition(state4, state4, std::move(state_4_4));

	TupleVectorPtr state_4_T = nullptr;
	AddTransition(state4, terminalState, std::move(state_4_T));
	//STATE 4 COMPLETE

	//STATE 5 INITIALIZING
	TupleVectorPtr state_5_5 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('0', '9'),
	}));
	AddTransition(state5, state5, std::move(state_5_5));

	TupleVectorPtr state_5_6 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('.', '.'),
	}));
	AddTransition(state5, state6, std::move(state_5_6));

	TupleVectorPtr state_5_0 = nullptr;
	AddTransition(state5, state0, std::move(state_5_0));
	//STATE 5 COMPLETE

	//STATE 6 INITIALIZING
	TupleVectorPtr state_6_6 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('0', '9'),
	}));
	AddTransition(state6, state6, std::move(state_6_6));

	TupleVectorPtr state_6_0 = nullptr;
	AddTransition(state6, state0, std::move(state_6_0));
	//STATE 6 COMPLETE

	//STATE 7 INITIALIZING
	TupleVectorPtr state_7_7 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple(-128, ('\'' - 1)),
		std::make_tuple(('\'' + 1), ('\\' - 1)),
		std::make_tuple(('\\' + 1), 127),
	}));
	AddTransition(state7, state7, std::move(state_7_7));

	TupleVectorPtr state_7_8 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('\\', '\\'),
	}));
	AddTransition(state7, state8, std::move(state_7_8));

	TupleVectorPtr state_7_12 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('\'', '\''),
	}));
	AddTransition(state7, state12, std::move(state_7_12));
	//STATE 7 COMPLETE

	//STATE 8 INITIALIZING
	TupleVectorPtr state_8_7 = nullptr;
	AddTransition(state8, state7, std::move(state_8_7));
	//STATE 8 COMPLETE

	//STATE 9 INITIALIZING
	TupleVectorPtr state_9_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('=', '='),
	}));
	AddTransition(state9, state0, std::move(state_9_0));

	TupleVectorPtr state_9_T = nullptr;
	AddTransition(state9, terminalState, std::move(state_9_T));
	//STATE 9 COMPLETE

	//STATE 10 INITIALIZING
	TupleVectorPtr state_10_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('>', '>'),
		std::make_tuple('=', '=')
	}));
	AddTransition(state10, state0, std::move(state_10_0));

	TupleVectorPtr state_10_T = nullptr;
	AddTransition(state10, terminalState, std::move(state_10_T));
	//STATE 10 COMPLETE

	//STATE 11 INITIALIZING
	TupleVectorPtr state_11_7 = nullptr;
	AddTransition(state11, state7, std::move(state_11_7));
	//STATE 11 COMPLETE

	//STATE 12 INITIALIZING
	TupleVectorPtr state_12_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('\'', '\'')
	}));
	AddTransition(state12, state0, std::move(state_12_0));
	//STATE 12 COMPLETE

	//STATE 0 INITIALIZING
	TupleVectorPtr state_0_T = nullptr;
	AddTransition(state0, terminalState, std::move(state_0_T));
	//STATE 0 COMPLETE

	currentState = startState;
}

void LexerFSM::AddTransition(StateNodePtr from, StateNodePtr to, TupleVectorPtr charVector) {
	from->nextStates.push_back(to);
	from->nextStateCodes.push_back(std::move(charVector));
}

LexerFSM::~LexerFSM() {

}

StateType LexerFSM::NextState(char ch) {
	StateNodePtr cur = currentState;
	StateCodesVector& codeVector = cur->nextStateCodes;

	auto setCurState = [this](int i) {
		currentState = currentState->nextStates[i].lock();
		return currentState->state;
	};

	for (size_t i = 0; i < codeVector.size(); i++) {
		if (codeVector[i] == nullptr) {
			return setCurState(i);
		}

		for (size_t j = 0; j < codeVector[i]->size(); j++) {
			auto [rightChar, leftChar] = codeVector[i]->at(j);

			if (ch >= rightChar && ch <= leftChar) {
				return setCurState(i);
			}
		}
	}

	currentState = terminalState;
	return currentState->state;
}

void LexerFSM::Reset() {
	currentState = startState;
}