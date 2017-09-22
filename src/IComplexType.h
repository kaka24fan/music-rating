/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "IType.h"
#include "IItem.h"

template<class T>
class IComplexType
	: public IType<T>
	, public IItem
{
public:

	// instance of a complex type should also override write(), writeHere() from IType<T>

	virtual void writeFull(Address a) = 0;
	virtual void writeFullHere() = 0;
};