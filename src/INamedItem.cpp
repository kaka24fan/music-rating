#include "INamedItem.h"
#include "TypeId.h"
#include "TypeName.h"

Name INamedItem::readName()
{
	PageIndex itemPage;
	if (!File::i()->getPageContainingId(itemPage, TypeId(m_id)))
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

void INamedItem::writeName(Name name)
{
}
