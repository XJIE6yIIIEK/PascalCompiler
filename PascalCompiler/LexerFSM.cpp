#include "LexerFSM.h"
#include "CustomExceptions.h"

StateNode::StateNode(StateType state, LexemType type = LexemType::Null) : state(state), type(type) {}

LexerFSM::LexerFSM() : startState(std::make_shared<StateNode>(StateType::Start)), terminalState(std::make_shared<StateNode>(StateType::Terminal)) {
	StateNodePtr state0 = std::make_shared<StateNode>(StateType::State0);
	StateNodePtr state1 = std::make_shared<StateNode>(StateType::State1);
	StateNodePtr state2 = std::make_shared<StateNode>(StateType::State2);
	StateNodePtr state3 = std::make_shared<StateNode>(StateType::State3, LexemType::Commentary);
	StateNodePtr state4 = std::make_shared<StateNode>(StateType::State4, LexemType::KeywordOrIdent);
	StateNodePtr state5 = std::make_shared<StateNode>(StateType::State5, LexemType::Integer);
	StateNodePtr state6 = std::make_shared<StateNode>(StateType::State6, LexemType::Float);
	StateNodePtr state7 = std::make_shared<StateNode>(StateType::State7, LexemType::Text);
	StateNodePtr state8 = std::make_shared<StateNode>(StateType::State8);
	StateNodePtr state9 = std::make_shared<StateNode>(StateType::State9);
	StateNodePtr state10 = std::make_shared<StateNode>(StateType::State10);
	StateNodePtr state11 = std::make_shared<StateNode>(StateType::State11, LexemType::Commentary);
	StateNodePtr state12 = std::make_shared<StateNode>(StateType::State12);
	StateNodePtr state13 = std::make_shared<StateNode>(StateType::State13, LexemType::Commentary);
	StateNodePtr state14 = std::make_shared<StateNode>(StateType::State14, LexemType::Commentary);
	StateNodePtr state15 = std::make_shared<StateNode>(StateType::State15, LexemType::Text);
	StateNodePtr state16 = std::make_shared<StateNode>(StateType::State16);
	StateNodePtr state17 = std::make_shared<StateNode>(StateType::State17, LexemType::KeywordOrIdent);
	StateNodePtr state18 = std::make_shared<StateNode>(StateType::State18);
	StateNodePtr stateErrUnkSy = std::make_shared<StateNode>(StateType::ErrorUnkwownSymbol);
	StateNodePtr stateErrWrngName = std::make_shared<StateNode>(StateType::ErrorWrongName);
	StateNodePtr stateErrWrngNum = std::make_shared<StateNode>(StateType::ErrorWrongNumber);

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
	statesVector.push_back(state13);
	statesVector.push_back(state14);
	statesVector.push_back(state15);
	statesVector.push_back(state16);
	statesVector.push_back(state17);
	statesVector.push_back(state18);
	statesVector.push_back(stateErrUnkSy);
	statesVector.push_back(stateErrWrngName);
	statesVector.push_back(stateErrWrngNum);

	//START INITIALIZING
	TupleVectorPtr state_Start_1 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('.', '.')
	}));
	AddTransition(startState, state1, std::move(state_Start_1));

	TupleVectorPtr state_Start_2 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('(', '(')
	}));
	AddTransition(startState, state2, std::move(state_Start_2));

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

	TupleVectorPtr state_Start_12 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('\'', '\'')
	}));
	AddTransition(startState, state12, std::move(state_Start_12));

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
		std::make_tuple(')', ')'),
		std::make_tuple('*', '*')
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

	TupleVectorPtr state_Start_E = nullptr;
	AddTransition(startState, stateErrUnkSy, std::move(state_Start_E));
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
	TupleVectorPtr state_2_13 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('*', '*')
	}));
	AddTransition(state2, state13, std::move(state_2_13));

	TupleVectorPtr state_2_T = nullptr;
	AddTransition(state2, terminalState, std::move(state_2_T));
	//STATE 2 COMPLETE

	//STATE 3 INITIALIZING
	TupleVectorPtr state_3_14 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('*', '*')
	}));
	AddTransition(state3, state14, std::move(state_3_14));

	TupleVectorPtr state_3_3 = nullptr;
	AddTransition(state3, state3, std::move(state_3_3));
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

	TupleVectorPtr state_5_16 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('.', '.'),
	}));
	AddTransition(state5, state16, std::move(state_5_16));

	TupleVectorPtr state_5_E = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('a', 'z'),
		std::make_tuple('A', 'Z'),
		std::make_tuple('_', '_')
	}));
	AddTransition(state5, stateErrWrngName, std::move(state_5_E));

	TupleVectorPtr state_5_T = nullptr; // 5 - 0
	AddTransition(state5, terminalState, std::move(state_5_T));
	//STATE 5 COMPLETE

	//STATE 6 INITIALIZING
	TupleVectorPtr state_6_6 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('0', '9'),
	}));
	AddTransition(state6, state6, std::move(state_6_6));

	TupleVectorPtr state_6_E = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('a', 'z'),
		std::make_tuple('A', 'Z'),
		std::make_tuple('_', '_')
		}));
	AddTransition(state6, stateErrWrngNum, std::move(state_6_E));

	TupleVectorPtr state_6_T = nullptr; // 6 - 0
	AddTransition(state6, terminalState, std::move(state_6_T));
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

	TupleVectorPtr state_7_15 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('\'', '\''),
	}));
	AddTransition(state7, state15, std::move(state_7_15));
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
	TupleVectorPtr state_11_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple(')', ')')
	}));
	AddTransition(state11, state0, std::move(state_11_0));
	//STATE 11 COMPLETE

	//STATE 12 INITIALIZING
	TupleVectorPtr state_12_7 = nullptr;
	AddTransition(state12, state7, std::move(state_12_7));
	//STATE 12 COMPLETE

	//STATE 13 INITIALIZING
	TupleVectorPtr state_13_3 = nullptr;
	AddTransition(state13, state3, std::move(state_13_3));
	//STATE 13 COMPLETE

	//STATE 14 INITIALIZING
	TupleVectorPtr state_14_11 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('*', '*')
	}));
	AddTransition(state14, state11, std::move(state_14_11));
	//STATE 14 COMPLETE

	//STATE 15 INITIALIZING
	TupleVectorPtr state_15_0 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('\'', '\'')
	}));
	AddTransition(state15, state0, std::move(state_15_0));
	//STATE 15 COMPLETE

	//STATE 16
	TupleVectorPtr state_16_6 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('0', '9')
	}));
	AddTransition(state16, state6, std::move(state_16_6));

	TupleVectorPtr state_16_17 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('.', '.'),
	}));
	AddTransition(state16, state17, std::move(state_16_17));

	TupleVectorPtr state_16_E = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('a', 'z'),
		std::make_tuple('A', 'Z'),
		std::make_tuple('_', '_')
	}));
	AddTransition(state16, stateErrWrngNum, std::move(state_16_E));
	//STATE 16 COMPLETE

	//STATE 17
	TupleVectorPtr state_17_18 = std::make_unique<TupleVector>(TupleVector({
		std::make_tuple('.', '.')
	}));
	AddTransition(state17, state18, std::move(state_17_18));

	TupleVectorPtr state_17_E = nullptr;
	AddTransition(state17, stateErrWrngNum, std::move(state_17_E));
	//STATE 17 COMPLETE

	//STATE 18
	TupleVectorPtr state_18_T = nullptr;
	AddTransition(state18, terminalState, std::move(state_18_T));
	//STATE 18 COMPLETE

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

void LexerFSM::ThrowError(PositionPtr pos) {
	StateType errState = currentState->state;
	Reset();

	switch (errState) {
		case StateType::ErrorUnkwownSymbol: {
			throw WrongSymbol::CreateException(std::move(pos));
		}; break;

		case StateType::ErrorWrongName: {
			throw WrongName::CreateException(std::move(pos));
		}; break;

		case StateType::ErrorWrongNumber: {
			throw WrongNumber::CreateException(std::move(pos));
		}; break;
	}
}