#pragma once

#include <map>

#define NO_STATE 0
#define RESERVED_TRIGGER_DEFAULT_ENTRY -1
#define RESERVED_TRIGGER_DEFAULT_EXIT -2

class State
{
public:
	virtual void EntryAction() = 0;
	virtual void ExitAction() = 0;
	unsigned int virtual Trigger(int trigger) = 0;
};

template <class T>
class StateTemplate : public State
{

protected:	
	typedef unsigned int (T::* Guard)(unsigned int);
	T* _derivedState;
	std::map<unsigned int, Guard> _triggers;

public:	
	
	unsigned int virtual Trigger(int trigger)
	{
		switch(trigger)
		{
		case RESERVED_TRIGGER_DEFAULT_EXIT:
			return RESERVED_TRIGGER_DEFAULT_EXIT;
		default:
		{
			if (_triggers.find(trigger) == _triggers.end())
			{
				return NO_STATE; // Not supported transition.
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

	void ChangeState(unsigned int newState)
	{
		if (_currentState != NULL)
		{
			_currentState->ExitAction();
		}

		if (_childStates.find(newState) != _childStates.end())
		{
			_currentState = _childStates[newState];
			_currentState->EntryAction();
		}
	}

protected:
	State* _currentState;

public:
	void AddState(unsigned int enumValue, State& instance)
	{
		_childStates.emplace(enumValue, &instance);
	}

	unsigned int virtual Trigger(int trigger)
	{
		switch (trigger)
		{
		case RESERVED_TRIGGER_DEFAULT_ENTRY:
		{
			if (_currentState != NULL)
			{
				return RESERVED_TRIGGER_DEFAULT_ENTRY;
			}
			ChangeState(_defaultEntryState);
		}
		break;
		case RESERVED_TRIGGER_DEFAULT_EXIT:
		{
			_currentState = NULL;
		}
		break;
		default:
		{
			if (_currentState != NULL)
			{
				unsigned int newState = _currentState->Trigger(trigger);
				ChangeState(newState);
			}
		}
		break;
		}
		
		return StateTemplate<T>::Trigger(trigger);
	}
};