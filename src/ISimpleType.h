#pragma once

#include "stdafx.h"

template<class T>
class ISimpleType : public IType<T>
{
	virtual bool isZero() = 0;
	virtual void writeZero(Address a) = 0; // should be static but can't have static and virtual at once?
	virtual void writeZeroHere() = 0;      // should be static but can't have static and virtual at once?
};