/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "TypeRatingData.h"
#include "File.h"

TypeRatingData::TypeRatingData(RatingData val)
{
	this->m_val = val;
}

TypeRatingData TypeRatingData::constructFromAddress(Address a)
{
	TypeRatingData result(0);
	result.read(a);
	return result;
}

TypeRatingData TypeRatingData::construct(unsigned short score)
{
	if (score < MIN_RATING)
	{
		score = MIN_RATING;
	}
	if (score > MAX_RATING)
	{
		score = MAX_RATING;
	}

	time_t current = time(0);
	current /= 60; // minutes since 1970
	assert(current < (1ull << 32)); // assert it can fit in 32 bits
	RatingData val = current << 32;

	for (unsigned int i = 0; i < sizeof(score) * 8; i++)
	{
		val &= ((score & 1) << i);
		score >>= 1;
	}

	return TypeRatingData(val);
}

void TypeRatingData::writeTerminatorOfMyType(Address a)
{
	File::i()->seek(a);
	writeTerminatorOfMyTypeHere();
}

void TypeRatingData::writeTerminatorOfMyTypeHere()
{
	TypeRatingData zeroRatingVal(0);
	zeroRatingVal.writeHere();
}

bool TypeRatingData::isZeroTerminator(Address a)
{
	TypeRatingData test = constructFromAddress(a);
	return test.m_val == 0;
}

void TypeRatingData::read(Address a)
{
	File::i()->seek(a);
	readHere();
}

void TypeRatingData::write(Address a)
{
	File::i()->seek(a);
	writeHere();
}

void TypeRatingData::readHere()
{
	RatingData val = 0;
	for (unsigned int i = 0; i < sizeof(Address) * 8; i++)
	{
		bool nextBit = File::i()->readNextItemBit();
		val = (val >> 1) + (nextBit ? 1 : 0);
	}
	m_val = val;
}

void TypeRatingData::writeHere()
{
	RatingData val = m_val;
	for (int i = sizeof(Address) * 8 - 1; i >= 0; i--)
	{
		File::i()->writeNextItemBit(val & 1);
		val >>= 1;
	}
}