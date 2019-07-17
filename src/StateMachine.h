/*
 * StateMachine.h:
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
#pragma once

// These reserved defines must be define in the enumeration that
// defines the state for your own state machine. NO_STATE is
// the value the state mcahines's current state member field is
// set to before default entry has occured or after default
// exit has occurred and represents any undefined state.
//
// enum class STATES
// {
// NOSTATE = RESERVED_NO_STATE,
// NOSTATECHANGE = RESERVED_NO_STATE_CHANGE,
// YOURSTATE1 = 0,
// YOURSTATE2,
// ...
// Count
// };
#define RESERVED_NO_STATE -1
#define RESERVED_NO_STATE_CHANGE -2

// The following reserved defines must be define in the
// enumeration that defines the triggers for your own
// state machine. 
//
// enum class TRIGGERS
// {
// 	DEFAULTENTRY = RESERVED_TRIGGER_DEFAULT_ENTRY,
// 	DEFAULTEXIT = RESERVED_TRIGGER_DEFAULT_EXIT,
// 	YOURTRIGGER1 = 0,
// 	YOURTRIGGER2,
// 	Count
// };
#define RESERVED_TRIGGER_DEFAULT_ENTRY -1
#define RESERVED_TRIGGER_DEFAULT_EXIT -2

class State
{
public:
	virtual void EntryAction() {};
	virtual void ExitAction() {};
};

template <class T, typename EnumTrigger, int countTriggers, typename EnumState>
class StateTemplate : public State
{

protected:	
	typedef EnumState (T::* Guard)(EnumTrigger);
	Guard _triggers[countTriggers];

public:
	StateTemplate()
	{
		for (int i = 0; i < countTriggers; i++)
		{
			_triggers[i] = nullptr;
		}
	}

	virtual EnumState EntryAction(EnumState currentState)
	{
		EntryAction();
		return EnumState::NOSTATECHANGE;
	}

	void EntryAction() override
	{
		State::EntryAction();
	}
	
	EnumState virtual Trigger(EnumTrigger trigger)
	{
		switch(trigger)
		{
		case EnumTrigger::DEFAULTENTRY:
		case EnumTrigger::DEFAULTEXIT:
			return EnumState::NOSTATE;
		default:
		{
			Guard guard = _triggers[(int) trigger];

			if (guard == nullptr)
				return EnumState::NOSTATE;

			return ((T*)this->*guard)(trigger);
		}
		break;
		}
	}

	void AddTriggerGuard(EnumTrigger trigger, Guard guard)
	{
		_triggers[(int) trigger] = guard;
	}
};

template <class T, typename EnumTrigger, int numTriggers, typename EnumState, int numStates, EnumState defaultEntryState>
class OrState : public StateTemplate<T, EnumTrigger, numTriggers, EnumState>
{

private:
	State* _childStates[numStates];
	EnumState _defaultEntryState = defaultEntryState;
	EnumState _currentState = EnumState::NOSTATE;

	void ChangeState(EnumState newState)
	{
		if (newState == EnumState::NOSTATECHANGE)
		{
			return;
		}

		if (_currentState != EnumState::NOSTATE)
		{
			_childStates[(int) _currentState]->ExitAction();
		}

		if (newState == EnumState::NOSTATE)
		{
			_currentState = EnumState::NOSTATE;
		}
		else
		{
			_currentState = newState;			
			EnumState nextState = ((StateTemplate<T, EnumTrigger, numTriggers, EnumState>*) _childStates[(int) _currentState])->EntryAction(_currentState);

			if (nextState != EnumState::NOSTATECHANGE)
			{
				ChangeState(nextState);
			}
		}
	}

public:
	OrState()
	{
		for (int i = 0; i < numStates; i++)
		{
			_childStates[i] = nullptr;
		}
	}
	
	void AddState(EnumState enumValue, State& instance)
	{		
		_childStates[(int) enumValue] = &instance;
	}

	void EntryAction() override
	{
		Trigger(EnumTrigger::DEFAULTENTRY);
	}

	void ExitAction() override
	{
		Trigger(EnumTrigger::DEFAULTEXIT);
	}

	EnumState GetCurrentState() { return _currentState; }

	EnumState Trigger(EnumTrigger trigger) override
	{
		switch (trigger)
		{
		case EnumTrigger::DEFAULTENTRY:
		{
			if (_currentState != EnumState::NOSTATE)
			{
				return EnumState::NOSTATECHANGE;
			}
			ChangeState(_defaultEntryState);
		}
		break;
		case EnumTrigger::DEFAULTEXIT:
		{			
			ChangeState(EnumState::NOSTATE);
		}
		break;
		default:
		{
			if (_currentState != EnumState::NOSTATE)
			{
				EnumState newState = ((StateTemplate<T, EnumTrigger, numTriggers, EnumState>*)_childStates[(int) _currentState])->Trigger(trigger);
				ChangeState(newState);
			}
		}
		break;
		}

		return StateTemplate<T, EnumTrigger, numTriggers, EnumState>::Trigger(trigger);
	}
};