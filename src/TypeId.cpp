#pragma once

#include "stdafx.h"

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

Byte TypeId::getFlags()
{	
	Byte result(m_val);
	result &= 15;
	return result;
}

ItemType TypeId::getItemType()
{
	return ItemType(m_val >> 28);
}

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
