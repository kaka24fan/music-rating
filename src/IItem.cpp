/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "IItem.h"
#include "TypeId.h"

IItem::IItem(Id id)
{
	m_id = id;
}

void IItem::assignAddress()
{
	// just for ease of use in this function
	File* file = File::i();

	// make sure we haven't assigned a page to this id before already!
	PageIndex dummy(0);
	assert(!file->getPageContainingId(dummy, TypeId(m_id)));

	PageIndex ourPage = file->getNextFreePage();
	file->initializePage(ourPage, m_id);
}
