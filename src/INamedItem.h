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
};