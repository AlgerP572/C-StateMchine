/*
 * Main.cpp:
 *	Base classes to support a C++ UML state machine.
 *	Copyright (c) 2019 Alger Pike
 ***********************************************************************
 * This file is part of CPlusPLusSateMachine:
 *	https://github.com/AlgerP572/CPlusPlusStateMchine
 *
 *    CPlusPLusSateMachine is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    CPlusPLusSateMachine is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with CPlusPLusSateMachine.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
*/
#include "StateMachine.h"

enum class DEFAULT_STATES { DEFAULT_ENTRY = -1, NO_STATE = -2};

enum class STATES
{
	NOSTATE = RESERVED_NO_STATE,
	NOSTATECHANGE = RESERVED_NO_STATE_CHANGE,
	IDLE = 0,
	FINAL,
	Count
};

enum class TRIGGERS 
{
	DEFAULTENTRY = RESERVED_TRIGGER_DEFAULT_ENTRY,
	DEFAULTEXIT = RESERVED_TRIGGER_DEFAULT_EXIT,
	IDLETRIGGER = 0,
	FINALTRIGGER,
	Count
};

class Idle : public StateTemplate<Idle,
	TRIGGERS,
	(int) TRIGGERS::Count,
	STATES>
{
private:
	STATES IdleTriggerGuard(TRIGGERS trigger);
	STATES FinalTriggerGuard(TRIGGERS trigger);

public:
	Idle();
	void virtual EntryAction() {};
	void virtual ExitAction() {};
};

Idle::Idle()
{	
	AddTriggerGuard(TRIGGERS::IDLETRIGGER, &Idle::IdleTriggerGuard);
	AddTriggerGuard(TRIGGERS::FINALTRIGGER, &Idle::FinalTriggerGuard);
}

STATES Idle::IdleTriggerGuard(TRIGGERS trigger)
{
	return STATES::IDLE;
}

STATES Idle::FinalTriggerGuard(TRIGGERS trigger)
{
	return STATES::FINAL;
}

class Final : public StateTemplate<Final,
	TRIGGERS,
	(int) TRIGGERS::Count,
	STATES>
{
private:
	STATES IdleTriggerGuard(TRIGGERS trigger);

public:
	Final();
	void virtual EntryAction() {};
	void virtual ExitAction() {};
};

Final::Final()
{
	AddTriggerGuard(TRIGGERS::IDLETRIGGER, &Final::IdleTriggerGuard);
}

STATES Final::IdleTriggerGuard(TRIGGERS trigger)
{
	return STATES::IDLE;
}

class SimpleStateMachine : public OrState<SimpleStateMachine,
	TRIGGERS,
	(int)TRIGGERS::Count,
	STATES,
	(int) STATES::Count,
	STATES::IDLE>
{
public:
	SimpleStateMachine();

	void virtual EntryAction();
	void virtual ExitAction();
};

SimpleStateMachine::SimpleStateMachine()
{
	Idle* idle = new Idle();
	Final* final = new Final();

	AddState(STATES::IDLE, *idle);
	AddState(STATES::FINAL, *final);
}

void SimpleStateMachine::EntryAction()
{
}

void SimpleStateMachine::ExitAction()
{
}

int main(void)
{	
	SimpleStateMachine sm;

	STATES stateNow = sm.GetCurrentState();

	// Init done move to intial state.
	sm.Trigger(TRIGGERS::DEFAULTENTRY);
	stateNow = sm.GetCurrentState();

	sm.Trigger(TRIGGERS::IDLETRIGGER);
	stateNow = sm.GetCurrentState();
	
	sm.Trigger(TRIGGERS::FINALTRIGGER);
	stateNow = sm.GetCurrentState();

	sm.Trigger(TRIGGERS::IDLETRIGGER);
	stateNow = sm.GetCurrentState();

	sm.Trigger(TRIGGERS::DEFAULTEXIT);
	stateNow = sm.GetCurrentState();
	return 0;
}