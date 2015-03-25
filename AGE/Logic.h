#ifndef __AGE_LOGIC_HEADER__
#define __AGE_LOGIC_HEADER__

#include <vector>

namespace AGE{


	class Logic;

	class LogicEvent
	{
	public:
		virtual int GetEventType() = 0;
	};

	class LogicState
	{
	public:
		LogicState(int id, Logic* logic);
		virtual ~LogicState() {}

		int GetID() { return mID; }

		virtual void Enter() = 0;
		virtual void React(LogicEvent* evt) {}
		virtual void Update(float deltaTime) = 0;
		virtual void Exit() = 0;

	protected:
		Logic* mLogic;
		int mID;
	};

	class Logic
	{
	public:
		~Logic();
		
		void Transit(int id);

		template<class State, typename ...Args>
		void NewState(Args&&... Param)
		{
			mStates.push_back(new State(std::forward<Args>(Param)...));
		}

		LogicState* GetCurrentState() { return mCurrentState; }

		void SetInitialState(int id) 
		{
			if (!mCurrentState) 
			{
				mCurrentState = FindState(id);
				mCurrentState->Enter();
			}

		}
	protected:

		LogicState* FindState(int id);

		LogicState* mCurrentState = 0;
		std::vector<LogicState*> mStates;
	};
}

#endif
