#pragma once

#include <map>

#define RESERVED_NO_STATE -1
#define RESERVED_NO_STATE_CHANGE -2

#define RESERVED_TRIGGER_DEFAULT_ENTRY -1
#define RESERVED_TRIGGER_DEFAULT_EXIT -2

class State
{
public:
	virtual void EntryAction() = 0;
	virtual void ExitAction() = 0;	
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
			_triggers[i] = NULL;
		}
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

			if (guard == NULL)
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
			_childStates[(int) _currentState]->EntryAction();
		}
	}

public:
	OrState()
	{
		for (int i = 0; i < numStates; i++)
		{
			_childStates[i] = NULL;
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