#pragma once

#include "stdafx.h"
#include "IItem.h"

class INamedItem : public IItem
{
public:
	Name readName();
	void writeName(Name name);
};