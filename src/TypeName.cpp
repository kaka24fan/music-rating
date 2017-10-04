/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "TypeName.h"
#include "File.h"

TypeName::TypeName(Name val)
{
	this->m_val = val;
}

TypeName TypeName::constructFromAddress(Address a)
{
	TypeName result(0);
	result.read(a);
	return result;
}

/*
TypeName is never part of a map or vector, thus this method should never be called.
*/
void TypeName::writeTerminatorOfMyType(Address a)
{
	assert(false);
}

/*
TypeName is never part of a map or vector, thus this method should never be called.
*/
void TypeName::writeTerminatorOfMyTypeHere()
{
	assert(false);
}

/*
TypeName is never part of a map or vector, thus this method should never be called.
*/
bool TypeName::isZeroTerminator(Address a)
{
	assert(false);
	return false;
}

void TypeName::read(Address a)
{
	File::i()->seek(a);
	readHere();
}

void TypeName::write(Address a)
{
	File::i()->seek(a);
	writeHere();
}

void TypeName::readHere()
{
	Name val = String();
	while (true)
	{
		Char nextChar = 0;
		for (unsigned int i = 0; i < sizeof(Char) * 8; i++)
		{
			bool nextBit;
			if (File::i()->readNextBit(nextBit))
			{
				nextChar = (nextChar >> 1) + (nextBit ? 1 : 0);
			}
		}
		if (nextChar != 0)
		{
			val.push_back(nextChar);
		}
		else
		{
			// end of string:
			break;
		}
	}
	m_val = val;
}

void TypeName::writeHere()
{
	for (Char c : m_val)
	{
		Char nextChar = c;
		bool buffer[sizeof(nextChar) * 8];
		for (unsigned int i = sizeof(nextChar) * 8 - 1; i >= 0; i--)
		{
			buffer[i] = (bool)(nextChar & 1);
			nextChar >>= 1;
		}
		
		for (bool bitsInRightOrder : buffer)
		{
			File::i()->writeNextBit(bitsInRightOrder);
		}
	}
}