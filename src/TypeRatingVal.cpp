#pragma once

#include "stdafx.h"

TypeRatingVal::TypeRatingVal(RatingVal val)
{
	this->m_val = val;
}

TypeRatingVal TypeRatingVal::constructFromAddress(Address a)
{
	TypeRatingVal result(0);
	result.read(a);
	return result;
}

void TypeRatingVal::writeTerminatorOfMyType(Address a)
{
	File::i()->seek(a);
	writeTerminatorOfMyTypeHere();
}

void TypeRatingVal::writeTerminatorOfMyTypeHere()
{
	TypeRatingVal zeroRatingVal(0);
	zeroRatingVal.writeHere();
}

bool TypeRatingVal::isZeroTerminator(Address a)
{
	TypeRatingVal test = constructFromAddress(a);
	return test.m_val == 0;
}

void TypeRatingVal::read(Address a)
{
	File::i()->seek(a);
	readHere();
}

void TypeRatingVal::write(Address a)
{
	File::i()->seek(a);
	writeHere();
}

void TypeRatingVal::readHere()
{
	RatingVal val = 0;
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

void TypeRatingVal::writeHere()
{
	RatingVal val = m_val;
	for (unsigned int i = sizeof(Address) * 8 - 1; i >= 0; i--)
	{
		File::i()->writeNextBit(val & 1);
		val >>= 1;
	}
}