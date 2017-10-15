/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "IItem.h"

class TypeId;

class INamedItem : public IItem
{
public:
	static Name readName(TypeId thisItemId);
	static void writeName(TypeId thisItemId, Name name);

	static void writeMemberId(TypeId thisItemId, TypeId memberId);
	static bool containsMemberId_WithFlagComparison(TypeId thisItemId, TypeId memberId);
	static std::vector<TypeId> getAllMemberIds(TypeId thisItemId);
};