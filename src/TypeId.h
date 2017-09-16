#pragma once

#include "stdafx.h"

typedef enum itemType
{
	// DON'T USE 0!
	IT_VEC = 1,
	IT_MAP = 2,
	IT_AUTHOR = 3,
	IT_ALBUM = 4,
	IT_ARTIST = 5,
	IT_SONG = 6,
	IT_SIMPLERATING = 7,
	IT_COMPOSITERATING = 8,
} ItemType;

/*
4 bits of ItemType, 24 bits of pure id, 4 bits of flags
*/
class TypeId : IType< Id >
{
public:
	TypeId(Id id);
	static TypeId constructFromAddress(Address a);

	Byte getFlags();
	ItemType getItemType();
	
	static bool equalityCheck(TypeId id1, TypeId id2);

	// Inherited via IType
	virtual void writeTerminatorOfMyType(Address a) override;
	virtual void writeTerminatorOfMyTypeHere() override;
	virtual bool isZeroTerminator(Address a) override;
	void read(Address a) override;
	void write(Address a) override;
	void readHere() override;
	void writeHere() override;

};