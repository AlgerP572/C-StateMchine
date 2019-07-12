#include "StateMachine.h"

enum STATES {IDLE = 1, FINAL = 2};
enum TRIGGERS { IDLETRIGGER = 1, FINALTRIGGER = 2 };

class Idle : public StateTemplate<Idle>
{
private:
	unsigned int TransitionToIdle(unsigned int);
	unsigned int TransitionToFinal(unsigned int);

public:
	Idle();
	void virtual EntryAction() {};
	void virtual ExitAction() {};
};

Idle::Idle()
{
	AddTriggerGuard(IDLETRIGGER, &Idle::TransitionToIdle);
	AddTriggerGuard(FINALTRIGGER, &Idle::TransitionToFinal);
}

unsigned int Idle::TransitionToIdle(unsigned int)
{
	return IDLE;
}

unsigned int Idle::TransitionToFinal(unsigned int)
{
	return FINAL;
}

class Final : public StateTemplate<Final>
{
private:
	unsigned int TransitionToIdle(unsigned int);

public:
	Final();
	void virtual EntryAction() {};
	void virtual ExitAction() {};
};

Final::Final()
{
	AddTriggerGuard(IDLETRIGGER, &Final::TransitionToIdle);
}

unsigned int Final::TransitionToIdle(unsigned int)
{
	return IDLE;
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
	Final* final = new Final();

	AddState(IDLE, *idle);
	AddState(FINAL, *final);
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
	sm.Trigger(IDLETRIGGER);

	sm.Trigger(FINALTRIGGER);
	return 0;
}