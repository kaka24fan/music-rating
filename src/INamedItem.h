/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "IItem.h"

class INamedItem : public IItem
{
public:
	Name readName();
	void writeName(Name name);
};