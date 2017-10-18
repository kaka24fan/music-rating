/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "TypeId.h"
#include "File.h"

TypeId::TypeId(Id id)
{
	this->m_val = id;
}

TypeId TypeId::constructFromAddress(Address a)
{
	TypeId result(0);
	result.read(a);
	return result;
}

void TypeId::read(Address a)
{
	File::i()->seek(a);
	readHere();
}

void TypeId::write(Address a)
{
	File::i()->seek(a);
	writeHere();
}

void TypeId::readHere()
{
	Id val = 0;
	for (unsigned int i = 0; i < sizeof(Id) * 8; i++)
	{
		bool nextBit = File::i()->readNextItemBit();
		val = (val << 1) + (nextBit ? 1 : 0);
	}
	m_val = val;
}

void TypeId::writeHere()
{
	Id val = m_val;
	for (int i = sizeof(Id) * 8 - 1; i >= 0; i--)
	{
		File::i()->writeNextItemBit(val & 1);
		val >>= 1;
	}
}

FlagType TypeId::getFlags()
{	
	unsigned int result = (unsigned int)m_val;
	result &= 15;

	// Only valid FlagType enumerations are 0, 1, 2, 3.
	assert(result < 4);
	return (FlagType)result;
}

ItemType TypeId::getItemType()
{
	return ItemType(m_val >> 28);
}

TypeId TypeId::getRawId()
{
	TypeId result = TypeId(m_val);
	result.setFlags((FlagType)0);
	result.setItemType((ItemType)0);
	return result;
}

void TypeId::setItemType(ItemType itemType)
{
	unsigned int bits = (unsigned int)itemType;
	m_val &= ~((~0) << 28); // clear the 4 most significant bits
	m_val |= (bits << 28); // overwrite them with the item type
}

void TypeId::setFlags(FlagType flags)
{
	unsigned int bits = (unsigned int)flags;
	m_val = (m_val >> 4) << 4; // clear the 4 least significant bits
	m_val |= bits; // overwrite them with the flags
}

/*
Does not compare the flags, but compares everything else.
*/
bool TypeId::equalityCheck(TypeId id1, TypeId id2)
{
	return (id1.m_val >> 4) == (id2.m_val >> 4);
}

/*
This function uses the basic readNextBit instead 
of readNextItemBit, because it is reading inside
page metadata, which is not handled at all by readNextItemBit.

Such a version only needs to be implemented for TypeId, because the other Type... items
do not appear in page metadata.
*/
void TypeId::readFromPageMetadata(Address a)
{
	File::i()->seek(a);

	Id val = 0;
	for (unsigned int i = 0; i < sizeof(Id) * 8; i++)
	{
		bool nextBit = File::i()->readNextBit();
		val = (val << 1) + (nextBit ? 1 : 0);
	}
	m_val = val;
}

/*
This function uses the basic writeNextBit instead
of writeNextItemBit, because it is writing inside
page metadata, which is not handled at all by writeNextItemBit.

Such a version only needs to be implemented for TypeId, because the other Type... items
do not appear in page metadata.
*/
void TypeId::writeToPageMetadata(Address a)
{
	File::i()->seek(a);

	Id val = m_val;
	for (int i = sizeof(Id) * 8 - 1; i >= 0; i--)
	{
		File::i()->writeNextBit(val & 1);
		val >>= 1;
	}
}

TypeId TypeId::constructFromAddressInPageMetadata(Address a)
{
	TypeId result(0);
	result.readFromPageMetadata(a);
	return result;
}

void TypeId::writeTerminatorOfMyType(Address a)
{
	File::i()->seek(a);
	writeTerminatorOfMyTypeHere();
}

void TypeId::writeTerminatorOfMyTypeHere()
{
	TypeId zeroId(0);
	zeroId.writeHere();
}

bool TypeId::isZeroTerminator(Address a)
{
	TypeId test = constructFromAddress(a);
	return test.m_val == 0;
}
