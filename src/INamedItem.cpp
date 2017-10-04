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
	assert(File::i()->getFirstDataBitOfPage(nameAddress, itemPage));
	nameAddress += sizeof(Id) * 8;
	TypeName name{Name()};
	name.read(nameAddress);
	return name.getValue();
}

/*
Prerequisite: there's a page owned by thisItemId.
*/
void INamedItem::writeName(TypeId thisItemId, Name name)
{
	PageIndex itemPage;
	if (!File::i()->getPageContainingId(itemPage, TypeId(thisItemId)))
	{
		// Item got constructed but has no page assigned :(
		assert(false);
	}
	Address nameAddress = 0;
	assert(File::i()->getFirstDataBitOfPage(nameAddress, itemPage));
	nameAddress += sizeof(Id) * 8;
	TypeName nameObj{ name };
	nameObj.write(nameAddress);
}
