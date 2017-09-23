/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "IItem.h"
#include "TypeId.h"

void IItem::assignAddress(TypeId thisItemId)
{
	// just grab handle for ease of use in this function
	File* file = File::i();

	// make sure we haven't assigned a page to this id before already!
	PageIndex dummy(0);
	assert(!file->getPageContainingId(dummy, thisItemId));

	PageIndex ourPage = file->getNextFreePage();
	file->initializePage(ourPage, thisItemId);
}
