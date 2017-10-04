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
	for (unsigned int i = 0; i < sizeof(Address) * 8; i++)
	{
		bool nextBit;
		if (File::i()->readNextBit(nextBit))
		{
			val = (val >> 1) + (nextBit ? 1 : 0);
		}
		else
		{
			// We got to the end of item's last page, we should have known there is nothing to read that far.
			assert(false);
		}
	}
	m_val = val;
}

void TypeId::writeHere()
{
	Id val = m_val;
	for (unsigned int i = sizeof(Address) * 8 - 1; i >= 0; i--)
	{
		File::i()->writeNextBit(val & 1);
		val >>= 1;
	}
}

FlagType TypeId::getFlags()
{	
	unsigned int result = (unsigned int)m_val;
	result &= 15;

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
