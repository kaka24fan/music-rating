/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "INamedItem.h"
#include "TypeId.h"
#include "TypeName.h"

Name INamedItem::readName(TypeId thisItemId)
{
	PageIndex itemPage;
	if (!File::i()->getPageContainingId(itemPage, TypeId(thisItemId)))
	{
		// Item got constructed but has no page assigned :(
		assert(false);
	}
	Address nameAddress = 0;
	File::i()->getFirstDataBitOfPage(nameAddress, itemPage);
	nameAddress += sizeof(Id) * 8;
	TypeName name{Name()};
	name.read(nameAddress);
	return name.getValue();
}

void INamedItem::writeName(TypeId thisItemId, Name name)
{
	// TODO
}
