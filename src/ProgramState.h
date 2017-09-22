/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "TypeId.h"

class ProgramState
{
public:
	TypeId getActiveUser();
	void logUserOut();
	void logUserIn(TypeId userId);
	bool isAnyUserLoggedIn();

	static ProgramState* i();

private:
	static ProgramState* m_instance;
	ProgramState();

	TypeId m_activeUserId;
};