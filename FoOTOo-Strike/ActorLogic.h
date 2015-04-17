#ifndef __AGE_ACTORLOGIC_HEADER__
#define __AGE_ACTORLOGIC_HEADER__


#include <Logic.h>
#include <AGE.h>

using namespace AGE;
using namespace OIS;

class KeyboardLogicEvent : public LogicEvent
{
public:
	int GetEventType()
	{
		return 0;
	}

	int KeyID;
};

class MouseLogicEvent : public LogicEvent
{
public:
	int GetEventType()
	{
		return 1;
	}

	MouseEvent mouseEvent;
};

class Actor;
class ActorLogicIdle : public LogicState
{
public:
	ActorLogicIdle(int id, Logic* logic, Actor* actor)
		:LogicState(id, logic), mActor(actor)
	{
	}

	void Enter() override;

	void React(LogicEvent* evt) override
	{
		//if (evt->GetEventType() == 1) {
		//	MouseLogicEvent* e = (MouseLogicEvent*)evt;
		//	e->mouseEvent.
		//}
	}

	void Update(float deltaTime) override;

	void Exit() override
	{

	}
private:
	Actor* mActor;
	MouseState state;
};

class ActorLogicReload : public LogicState
{
public:
	ActorLogicReload(int id, Logic* logic, Actor* actor)
		:LogicState(id, logic), mActor(actor)
	{
	}

	void Enter() override;

	void React(LogicEvent* evt) override
	{
	}

	virtual void Update(float deltaTime) override;

	virtual void Exit()
	{

	}
private:
	Actor* mActor;
};


class ActorLogicShoot1 : public LogicState
{
public:
	ActorLogicShoot1(int id, Logic* logic, Actor* actor)
		:LogicState(id, logic), mActor(actor)
	{
	}

	void Enter() override;

	void Update(float deltaTime) override;

	void Exit() override
	{

	}
private:
	Actor* mActor;
};


#endif
