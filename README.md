# CPlusPlusStateMchine
Simple State machine classes for UML state machine.

Classes that provide support for a UML state machine in C++.  Examples provided for the
state diagrams from: https://en.wikipedia.org/wiki/UML_state_machine

## State class

The state class is an abstract class the defines the interface or contract for a state.  The follwoing behaviors are provided:

### EntryAction()

The entry action is run anytime a trigger occurs that causes the state to be entered.  For hardware specific state machines the entry action would typically intialize any devices that the state may require. In general this is the Entry action that the class should override.

    void S11::ExitAction()
    {
	      printf("a() : ");
    }
    
If a state is required to support a triggerless transtion to the next source state it should override the EntryAction(EnumState& triggerless) overload. 

### EntryAction(EnumState& triggerless)

This overload of EntryAction() provides a mechanism where the new target state can support further transitions without an event to trigger the transition.  This type of transition is know in UML as a triggerless transtion.  To support the triggerless transition the class must override this specific overload of EntryAction(EnumState& triggerless).  The default implmentation of EntryAction(EnumState& triggerless) calls the EntryAction() overload and returns the reserved keyword RESERVED_NO_STATECHANGE which indicates to the state machine that no state change should take place:

    void EntryAction(EnumState& triggerless) override
	  {
		    EntryAction();
		    triggerless = EnumState::NOSTATECHANGE;
	  }
    
To enable a triggerless transtion override

### ExitAction()

The exit action is executed anytime a trigger occurs that causes a sate transition.  The current state of the state machine or the source state executes the exit action before the state machine transitions to the new target state.
