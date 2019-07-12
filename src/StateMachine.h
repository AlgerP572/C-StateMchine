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
	int virtual Trigger(int trigger) = 0;
};

template <class T>
class StateTemplate : public State
{

protected:	
	typedef unsigned int (T::* Guard)(unsigned int);	
	std::map<unsigned int, Guard> _triggers;

public:	
	
	int virtual Trigger(int trigger)
	{
		switch(trigger)
		{
		case RESERVED_TRIGGER_DEFAULT_EXIT:
			return RESERVED_NO_STATE;
		default:
		{
			if (_triggers.find(trigger) == _triggers.end())
			{
				return RESERVED_NO_STATE_CHANGE; // Not supported transition.
			}
			else
			{
				Guard guard = _triggers[trigger];
				return ((T*)this->*guard)(trigger);
			}
		}
		break;
		}
	}

	void AddTriggerGuard(unsigned int trigger, Guard guard)
	{
		_triggers.emplace(trigger, guard);
	}
};

template <class T, int U>
class OrState : public StateTemplate<T>
{

private:
	std::map<unsigned int, State*> _childStates;
	int _defaultEntryState = U;
	int _currentState;

	void ChangeState(unsigned int newState)
	{
		if (newState == RESERVED_NO_STATE_CHANGE)
		{
			return;
		}

		if (_currentState != RESERVED_NO_STATE)
		{
			_childStates[CurrentState]->ExitAction();
		}

		if (newState == RESERVED_NO_STATE)
		{
			_currentState = RESERVED_NO_STATE;
		}
		else if(_childStates.find(newState) != _childStates.end())
		{
			_currentState = newState;			
			_childStates[_currentState]->EntryAction();
		}
	}

public:
	
	void AddState(unsigned int enumValue, State& instance)
	{
		// Negative values are reserved for internal private
		// use so insigned int is by design here...
		_childStates.emplace(enumValue, &instance);
	}

	void virtual EntryAction()
	{
		Trigger(RESERVED_TRIGGER_DEFAULT_ENTRY);
	}

	void virtual ExitAction()
	{
		Trigger(RESERVED_TRIGGER_DEFAULT_EXIT);
	}

	int GetCurrentState() { return _currentState; }

	unsigned int virtual Trigger(int trigger)
	{
		switch (trigger)
		{
		case RESERVED_TRIGGER_DEFAULT_ENTRY:
		{
			if (_currentState != RESERVED_NO_STATE)
			{
				return RESERVED_NO_STATE_CHANGE;
			}
			ChangeState(_defaultEntryState);
		}
		break;
		case RESERVED_TRIGGER_DEFAULT_EXIT:
		{			
			ChangeState(RESERVED_NO_STATE);
		}
		break;
		default:
		{
			if (_currentState != RESERVED_NO_STATE)
			{
				unsigned int newState = _childStates[_currentState]->Trigger(trigger);
				ChangeState(newState);
			}
		}
		break;
		}
		
		return StateTemplate<T>::Trigger(trigger);
	}
};