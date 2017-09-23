/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "IItem.h"

class INamedItem : public IItem
{
public:
	Name readName(TypeId thisItemId);
	void writeName(TypeId thisItemId, Name name);
};