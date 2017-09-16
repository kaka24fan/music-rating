#pragma once

#include "stdafx.h"

class TypeRatingVal : public IType<RatingVal>
{
public:
	TypeRatingVal(RatingVal val);
	static TypeRatingVal constructFromAddress(Address a);

	// Inherited via IType
	virtual void writeTerminatorOfMyType(Address a) override;
	virtual void writeTerminatorOfMyTypeHere() override;
	virtual bool isZeroTerminator(Address a) override;
	void read(Address a) override;
	void write(Address a) override;
	void readHere() override;
	void writeHere() override;
};