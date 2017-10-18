/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "INamedItem.h"
#include "TypeId.h"
#include "TypeName.h"

Address getFirstBitOfMemberIds(TypeId thisItemId)
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
	TypeName name{ Name() };
	name.read(nameAddress);

	return File::i()->getNextBitToRW();
}

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
	if (!File::i()->getPageContainingId(itemPage, thisItemId))
	{
		// Item got constructed but has no page assigned :(
		assert(false);
	}
	Address nameAddress = 0;
	assert(File::i()->getFirstDataBitOfPage(nameAddress, itemPage));
	TypeName nameObj{ name };
	nameObj.write(nameAddress);
}

void INamedItem::writeMemberId(TypeId thisItemId, TypeId memberIdToWrite)
{
	Address idStart = getFirstBitOfMemberIds(thisItemId);
	File::i()->seek(idStart);
	// Now the file pointer points at the first bit of the first member id.

	TypeId memberId(0);
	do
	{
		memberId.readHere();
	} while (memberId.getValue() != 0);

	Address startOfTheNewId = File::i()->getNextBitToRW() - sizeof(Id) * 8;
	memberIdToWrite.write(startOfTheNewId);
}

bool INamedItem::containsMemberId_WithFlagComparison(TypeId thisItemId, TypeId targetMemberId)
{
	Address idStart = getFirstBitOfMemberIds(thisItemId);
	File::i()->seek(idStart);
	// Now the file pointer points at the first bit of the first member id.
	
	TypeId memberId(0);
	do
	{
		memberId.readHere();
		if (TypeId::equalityCheck(memberId, targetMemberId) && memberId.getFlags() == targetMemberId.getFlags())
			return true;
	} while (memberId.getValue() != 0);

	return false;
}

std::vector<TypeId> INamedItem::getAllMemberIds(TypeId thisItemId)
{
	Address idStart = getFirstBitOfMemberIds(thisItemId);
	File::i()->seek(idStart);
	// Now the file pointer points at the first bit of the first member id.

	std::vector<TypeId> result{};
	TypeId memberId(0); 
	while(true)
	{
		memberId.readHere();
		if (memberId.getValue() == 0) 
			return result;
		result.push_back(memberId);
	}
}
