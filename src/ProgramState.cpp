#include "ProgramState.h"

ProgramState* ProgramState::m_instance = nullptr;

ProgramState* ProgramState::i()
{
	if (!m_instance)
		m_instance = new ProgramState();

	return m_instance;
}

TypeId ProgramState::getActiveUser()
{
	return m_activeUserId;
}

void ProgramState::logUserOut()
{
	m_activeUserId = TypeId(0);
}

void ProgramState::logUserIn(TypeId userId)
{
	m_activeUserId = userId;
}

bool ProgramState::isAnyUserLoggedIn()
{
	return m_activeUserId.getValue() != 0;
}

ProgramState::ProgramState()
	: m_activeUserId(0)
{
}