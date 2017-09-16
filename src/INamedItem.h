#include "stdafx.h"

class INamedItem
{
public:
	virtual Name readName(Id itemId) = 0;
	virtual void writeName(Id itemId, Name name) = 0;
};