/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "IType.h"

#include <ctime>

static const int MIN_RATING = 0;
static const int MAX_RATING = 40;

class TypeRatingData : public IType<RatingData>
{
public:
	TypeRatingData(RatingData val);
	static TypeRatingData constructFromAddress(Address a);
	static TypeRatingData construct(unsigned short score);

	// Inherited via IType
	virtual void writeTerminatorOfMyType(Address a) override;
	virtual void writeTerminatorOfMyTypeHere() override;
	virtual bool isZeroTerminator(Address a) override;
	void read(Address a) override;
	void write(Address a) override;
	void readHere() override;
	void writeHere() override;
};