/*
 * Main.cpp:
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
#include "./SimpleStateMachine/SimpleStateMachine.h"

int main(void)
{	
	SimpleStateMachine sm;

	STATES stateNow = sm.GetCurrentState();

	// Init done move to intial state.
	sm.Trigger(TRIGGERS::DEFAULTENTRY);
	stateNow = sm.GetCurrentState();

	// Self-trigger IDLE to IDLE
	sm.Trigger(TRIGGERS::IDLETRIGGER);
	stateNow = sm.GetCurrentState();
	
	sm.Trigger(TRIGGERS::FINALTRIGGER);
	stateNow = sm.GetCurrentState();

	sm.Trigger(TRIGGERS::IDLETRIGGER);
	stateNow = sm.GetCurrentState();

	sm.Trigger(TRIGGERS::DEFAULTEXIT);
	stateNow = sm.GetCurrentState();
	return 0;
}