#include "stdafx.h"

class IItem
{
public:
	virtual Id readId() = 0;
	virtual void writeId(Id id) = 0;
};