/*
 * S1.cpp:
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
#include "S1.h"
#include "S11.h"
#include <stdio.h>

S1::S1()
{
	S11* s11 = new S11();
	
	AddState(SSTATES::S11, s11);

	AddTriggerGuard(STRIGGERS::T, &S1::TTriggerGuard);
}

void S1::TTriggerGuard(STRIGGERS trigger, Transition<S1, SSTATES>& transition)
{
	printf("g() : ");
	transition.TargetState = SSTATES::S2;
	transition.Actions = &S1::TTransition;
}

void S1::TTransition()
{	
	printf("t() : ");
}

void S1::ExitAction()
{
	// order is important here. According to UML the child state
	// needs to exit first so call the base class first.
	OrState::ExitAction();

	// Now perform Exit action for this state.
	printf("b() : ");	
}
