#pragma once

#include "stdafx.h"

template <class T>
class IType
{
public:

	/*
	This function is guaranteed to leave the file with its RW index pointing to the next bit after the last bit of this primitive value just read
	*/
	virtual void read(Address a) = 0;

	/*
	This function is guaranteed to leave the file with its RW index pointing to the next bit after the last bit of this primitive value just read
	*/
	virtual void readHere() = 0;

	/*
	This function is guaranteed to leave the file with its RW index pointing to the next bit after the last bit of this primitive value just written
	*/
	virtual void write(Address a) = 0;

	/*
	This function is guaranteed to leave the file with its RW index pointing to the next bit after the last bit of this primitive value just written
	*/
	virtual void writeHere() = 0;

	/*
	This function called from type T should be used to terminate a vector of T objects or a map of <T, V> pairs. Will write zero of appropriate length in bits.
	*/
	virtual void writeTerminatorOfMyType(Address a) = 0;

	/*
	This function called from type T should be used to terminate a vector of T objects or a map of <T, V> pairs. Will write zero of appropriate length in bits.
	*/
	virtual void writeTerminatorOfMyTypeHere() = 0;

	/*
	Returns true if reading in a value of this type starting from Address a would result in a vec/map terminator (zero).
	*/
	virtual bool isZeroTerminator(Address a) = 0;

	/*
	Returns the underlying value.
	*/
	T getValue() { return m_val; }

protected:
	T m_val;
};