/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"

typedef enum itemType
{
	IT_INVALID = 0,
	IT_VEC = 1,
	IT_MAP = 2,
	IT_USER = 3,
	IT_ALBUM = 4,
	IT_ARTIST = 5,
	IT_SONG = 6,
	IT_SIMPLERATING = 7,
	IT_COMPLEXRATING = 8,
} ItemType;

// Currently there is a check in TypeId::getFlags() to see if the cast is done on something in range...
// Do something better?
typedef enum flagType
{
	FT_INVALID = 0,
	FT_LEAD = 1,
	FT_FEATURING = 2,
	FT_PRODUCER = 3,
} FlagType;

// Can't define a static function in the .cpp
// And if I make it not static there is a multiple definition error that I couldn't solve quickly
static String itemTypeToString(ItemType itemType)
{
	switch (itemType)
	{
	case IT_INVALID:
		return String(L"Invalid ItemType");
	case IT_VEC:
		return String(L"Vector");
	case IT_MAP:
		return String(L"Map");
	case IT_USER:
		return String(L"User");
	case IT_ALBUM:
		return String(L"Album");
	case IT_ARTIST:
		return String(L"Artist");
	case IT_SONG:
		return String(L"Song");
	case IT_SIMPLERATING:
		return String(L"Simple Rating");
	case IT_COMPLEXRATING:
		return String(L"Complex Rating");
	default:
		return String(L"Very Invalid ItemType");
	}
}
static String flagTypeToString(FlagType flagType)
{
	switch (flagType)
	{
	case FT_INVALID:
		return String(L"Invalid FlagType");
	case FT_LEAD:
		return String(L"Lead");
	case FT_FEATURING:
		return String(L"Featuring");
	case FT_PRODUCER:
		return String(L"Producer");
	default:
		return String(L"Very Invalid FlagType");
	}
}

template <class T>
class IType
{
public:

	/*
	This function is guaranteed to leave the file with its RW index pointing to the next bit after the last bit of this primitive value just read
	*/
	virtual void read(Address a) = 0;

	/*
	This function is guaranteed to leave the file with its RW index pointing to the next bit after the last bit of this primitive value just read
	*/
	virtual void readHere() = 0;

	/*
	This function is guaranteed to leave the file with its RW index pointing to the next bit after the last bit of this primitive value just written
	*/
	virtual void write(Address a) = 0;

	/*
	This function is guaranteed to leave the file with its RW index pointing to the next bit after the last bit of this primitive value just written
	*/
	virtual void writeHere() = 0;

	/*
	This function called from type T should be used to terminate a vector of T objects or a map of <T, V> pairs. Will write zero of appropriate length in bits.
	*/
	virtual void writeTerminatorOfMyType(Address a) = 0;

	/*
	This function called from type T should be used to terminate a vector of T objects or a map of <T, V> pairs. Will write zero of appropriate length in bits.
	*/
	virtual void writeTerminatorOfMyTypeHere() = 0;

	/*
	Returns true if reading in a value of this type starting from Address a would result in a vec/map terminator (zero).
	*/
	virtual bool isZeroTerminator(Address a) = 0;

	/*
	Returns the underlying value.
	*/
	T getValue() { return m_val; }

protected:
	T m_val;
};