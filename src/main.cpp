#include "StateMachine.h"

enum STATES {IDLE = 1};
enum TRIGGERS { IDLETRIGGER = 1 };

class Idle : public StateTemplate<Idle>
{
private:
	unsigned int TransitionFromIdle(unsigned int);

public:
	Idle();
	void virtual EntryAction() {};
	void virtual ExitAction() {};
};

Idle::Idle()
{
	AddTriggerGuard(IDLETRIGGER, &Idle::TransitionFromIdle);
}

unsigned int Idle::TransitionFromIdle(unsigned int)
{
	return NO_STATE;
}

class SimpleStateMachine : public OrState<SimpleStateMachine, IDLE>
{
public:
	SimpleStateMachine();

	void virtual EntryAction();
	void virtual ExitAction();
};

SimpleStateMachine::SimpleStateMachine()
{
	Idle* idle = new Idle();
	AddState(IDLE, *idle);
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

	// Init done move to intial state.
	sm.Trigger(RESERVED_TRIGGER_DEFAULT_ENTRY);

	sm.Trigger(IDLETRIGGER);
	return 0;
}