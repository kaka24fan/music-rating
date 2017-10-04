/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "IType.h"
#include "File.h"

/*
4 bits of ItemType, 24 bits of pure id, 4 bits of flags
*/
class TypeId : public IType< Id >
{
public:
	TypeId(Id id);
	static TypeId constructFromAddress(Address a);

	FlagType getFlags();
	ItemType getItemType();
	
	// Gets the id with flags and itemtype cleared to zero.
	TypeId getRawId();

	void setItemType(ItemType itemType);
	void setFlags(FlagType flags);
	
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