#pragma once

#include <map>

#define RESERVED_TRIGGER_DEFAULT_ENTRY -1
#define RESERVED_TRIGGER_DEFAULT_EXIT -2

template <class T>
class State
{

protected:	
	typedef unsigned int (T::* Guard)(unsigned int);
	std::map<unsigned int, Guard> _triggers;

public:
	virtual void EntryAction() = 0;
	virtual void ExitAction() = 0;
	unsigned int virtual Trigger(int trigger)
	{
		if (_triggers.find(toState) == _triggers.end())
		{
			return 0; // Not supported transition.
		}
		else
		{
			Guard guard = _triggers[toState];
			return (this->*guard)(toState);
		}
	}
	void AddTriggerGuard(unsigned int trigger, Guard guard)
	{
		_triggers.emplace(trigger, guard);
	}
};

template <class T>
class OrState : public State<T>
{

private:
	std::map<unsigned int, State&> _childStates;

	void ChangeState(unsigned int newState)
	{
		if (_currentState != NULL)
		{
			_currentState->ExitAction();
		}

		if (_childStates.find(newState) != _childStates.end)
		{
			_currentState = &(_childStates[newState]);
			_currentState->EntryAction();
		}
	}

protected:
	State* _currentState;

public:
	void AddState(unsigned int enumValue, State& instance)
	{
		_childStates.emplace(enumValue, instance);
	}
	unsigned int virtual Trigger(int trigger)
	{
		if (_currentState != NULL)
		{
			unsigned int newState = _currentState->Trigger(trigger);
			ChangeState(newState);
		}
		return State::Trigger(trigger);
	}
};