/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

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

TypeId ProgramState::generateId(ItemType type)
{
	TypeId result = m_nextId;
	m_nextId = TypeId(m_nextId.getValue() + (1 << 4)); // 4 least significant bits are the flags

	if (m_nextId.getItemType() != result.getItemType())
	{
		// The ID data has overflown to the ItemType segment - i.e. we ran out of ID space (24 bits)
		assert(false);
	}

	return result;
}

ProgramState::ProgramState()
	: m_activeUserId(0)
	, m_nextId(0) // to be set properly below
{
	Id biggestUsedIdValueWithoutItemType = 0;

	for (PageIndex pageIndex = 1; pageIndex <= File::i()->getLastUsedPageIndex(); pageIndex++)
	{
		if (File::i()->isPageAFirstPage(pageIndex))
		{
			TypeId pageItemId = File::i()->readPageItemId(pageIndex);
			Id rawPageItemIdVal = pageItemId.getRawId().getValue();
			if (rawPageItemIdVal > biggestUsedIdValueWithoutItemType)
			{
				biggestUsedIdValueWithoutItemType = rawPageItemIdVal;
			}
		}
	}

	// Set the next id to be used for an id generation request.
	// Need to increment by (1 << 4) because the 4 least significant bits are flags.
	m_nextId = TypeId(biggestUsedIdValueWithoutItemType + (1 << 4));
}