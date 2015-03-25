#include "Logic.h"


using namespace AGE;

LogicState::LogicState(int id, Logic* logic)
	:mID(id), mLogic(logic)
{
}

Logic::~Logic()
{
	for (auto state : mStates)
	{
		delete state;
	}
}


void Logic::Transit(int id)
{
	LogicState* pointer = FindState(id);
	
	mCurrentState->Exit();
	pointer->Enter();
	mCurrentState = pointer;
}

LogicState* Logic::FindState(int id)
{
	for each(auto state in mStates)
	{
		if (state->GetID() == id) {
			return state;
		}
	}
	return 0;
}
