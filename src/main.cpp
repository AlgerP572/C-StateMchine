#include "StateMachine.h"

enum STATES {IDLE = 1};
enum TRIGGERS { IDLETRIGGER = 1 };

class Idle : public State<Idle>
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

}

class SimpleStateMachine : public OrState<SimpleStateMachine>
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

	sm.Trigger();
	return 0;
}