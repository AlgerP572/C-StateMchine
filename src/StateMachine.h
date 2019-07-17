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
	virtual ~State() {};
	virtual void EntryAction() {};
	virtual void ExitAction() {};
};

template <typename EnumState>
struct Transition final
{
	EnumState TargetState;

	typedef void (*TransitionAction)();
	TransitionAction Action;

	Transition() :
		TargetState(EnumState::NOSTATE),
		Action(nullptr)
	{
	}

	Transition(EnumState targetState) :
		TargetState(targetState),
		Action(nullptr)
	{
	}
};

template <class T, typename EnumTrigger, int countTriggers, typename EnumState>
class StateTemplate : public State
{
protected:	
	typedef void (T::* Guard)(EnumTrigger, Transition<EnumState>&);
	Guard _triggers[countTriggers];

public:
	StateTemplate()
	{
		for (int i = 0; i < countTriggers; i++)
		{
			_triggers[i] = nullptr;
		}
	}

	virtual void EntryAction(Transition<EnumState>& transition)
	{
		EntryAction();
		transition.TargetState = EnumState::NOSTATECHANGE;
		transition.Action = nullptr;
	}

	void EntryAction() override
	{
		State::EntryAction();
	}
	
	void virtual Trigger(EnumTrigger trigger, Transition<EnumState>& transition)
	{
		switch(trigger)
		{
		case EnumTrigger::DEFAULTENTRY:
		case EnumTrigger::DEFAULTEXIT:
		{
			transition.TargetState = EnumState::NOSTATE;
			transition.Action = nullptr;
		}
		default:
		{
			Guard guard = _triggers[(int) trigger];

			if (guard == nullptr)
			{
				transition.TargetState = EnumState::NOSTATE;
				transition.Action = nullptr;
			}
			
			((T*)this->*guard)(trigger, transition);
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
			Transition<EnumState> transition;
			((StateTemplate<T, EnumTrigger, numTriggers, EnumState>*) _childStates[(int) _currentState])->EntryAction(transition);

			if (transition.TargetState != EnumState::NOSTATECHANGE)
			{
				ChangeState(transition.TargetState);
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

	~OrState() override
	{
		for (int i = 0; i < numStates; i++)
		{
			State* pState = _childStates[i];

			if (pState == nullptr)
				continue;

			delete pState;
		}
	}
	
	void AddState(EnumState enumValue, State& instance)
	{		
		_childStates[(int) enumValue] = &instance;
	}

	void EntryAction() override
	{
		Transition<EnumState> transition;
		Trigger(EnumTrigger::DEFAULTENTRY, transition);
	}

	void ExitAction() override
	{
		Transition<EnumState> transition;
		Trigger(EnumTrigger::DEFAULTEXIT, transition);
	}

	EnumState GetCurrentState() { return _currentState; }

	void Trigger(EnumTrigger trigger, Transition<EnumState>& transition) override 
	{
		switch (trigger)
		{
		case EnumTrigger::DEFAULTENTRY:
		{
			if (_currentState != EnumState::NOSTATE)
			{
				transition.TargetState = EnumState::NOSTATECHANGE;
				transition.Action = nullptr;
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
				Transition<EnumState> transition;
				((StateTemplate<T, EnumTrigger, numTriggers, EnumState>*)_childStates[(int)_currentState])->Trigger(trigger, transition);

				if (transition.Action != nullptr)
				{
					transition.Action();
				}

				ChangeState(transition.TargetState);
			}
		}
		break;
		}

		return StateTemplate<T, EnumTrigger, numTriggers, EnumState>::Trigger(trigger, transition);
	}
};