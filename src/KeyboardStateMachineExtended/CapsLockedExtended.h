/*
 * CapsLockedExtended.h:
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
#pragma once

#include "KeyboardStatesTriggersExtended.h"
#include "KeyboardStateModel.h"
#include "../StateMachine.h"

class CapsLockedExtended : public StateTemplate<CapsLockedExtended,
	KEYBOARDTRIGGERSExtended,
	(int)KEYBOARDTRIGGERSExtended::Count,
	KEYBOARDSTATESExtended>
{
private:
	KeyboardStateModel& _stateModel;

	void CapsLockTriggerGuard(KEYBOARDTRIGGERSExtended trigger, Transition<CapsLockedExtended, KEYBOARDSTATESExtended>& transition);
	void AnyKeyTriggerGuard(KEYBOARDTRIGGERSExtended trigger, Transition<CapsLockedExtended, KEYBOARDSTATESExtended>& transition);

	void AnyKeyTransition();

public:
	CapsLockedExtended(KeyboardStateModel& stateModel);
	void EntryAction() override {};
	void ExitAction() override {};
};
