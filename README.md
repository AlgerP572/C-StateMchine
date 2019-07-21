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

    void State::EntryAction(EnumState& triggerless) override
    {
        EntryAction();
        triggerless = EnumState::NOSTATECHANGE;
    }
    
To enable a triggerless transtion override EntryAction(EnumState& triggerless) as follows:

    void S11::ExitAction(EnumState& triggerless)
    {
        printf("a() : ");
	triggerless = NEXTTARGETSTATE;
    }

### ExitAction()

The exit action is executed anytime a trigger occurs that causes a sate transition.  The current state of the state machine or the source state executes the exit action before the state machine transitions to the new target state.

### Trigger(EnumTrigger trigger)

Calling trigger allows the state an oppurtunity to examine the trigger and decide if the trigger should cause the state machine to transition to a new target state.  Each trigger is defined in a trigger enumeration and can be associated with a guard conditon for a transition.  The guard condition will execute and determine what the next target state of the state machine should be. Each state can setup its guard conditions for that triggers that the state supports as follows:

    Idle::Idle()
    {
        AddTriggerGuard(TRIGGERS::IDLETRIGGER, &Idle::IdleTriggerGuard);
        AddTriggerGuard(TRIGGERS::FINALTRIGGER, &Idle::FinalTriggerGuard);
    }
    
If a guard condition is not set up for a specific trigger it will be not be processed and simply be ignored by the state machine. When a guard condition is setup for the sepcific trigger its implemenation should provide what the next target state using the current conditions of the state model.

    void DefaultExtended::AnyKeyTriggerGuard(KEYBOARDTRIGGERSExtended trigger, Transition<DefaultExtended, KEYBOARDSTATESExtended>& transition)
    {
        if (_stateModel.GetKeyCount() > 0)
	{
	    transition.TargetState = KEYBOARDSTATESExtended::DEFAULT;

            std::function<void()> fn = [this]() { _stateModel.DecrementKeyCount(); };
	    transition.Actions = &DefaultExtended::AnyKeyTransition;
	}
	else
	{
	    transition.TargetState = KEYBOARDSTATESExtended::NOSTATE;
	}
    }
    
As shown above the target state can vary depending upon whether the maximum key count for the state machine has been exceeded.

### TransitionActions()

In general the TransitionActions() interface member should not need to be redefined but it is provided as virtual in case it would be required in some at this point unknown use case.  The TransitionActions() function is called during the sequence of a transition in the state machine which happens in the following sequence.

1. State machine ends a trigger
1. Guard for this trigger returns next target state
1. The exit action for the source state executes
1. The TransitionActions() now execute
1. The entry action for the target state now execute
1. Transition is complete and state machine waits for next trigger.



