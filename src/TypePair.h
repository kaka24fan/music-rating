#pragma once

#include "stdafx.h"

template<class T, class U>
class TypePair : IType< std::pair<T, U> >
{
public:
	// Inherited via IType
	virtual void writeTerminatorOfMyType(Address a) override;
	virtual void writeTerminatorOfMyTypeHere() override;
	virtual bool isZeroTerminator(Address a) override;
	void read(Address a) override;
	void write(Address a) override;
	void readHere() override;
	void writeHere() override;


	// Definitions:

	/*
	This should fail if m_val.first is not an IType.
	*/
	template<class T, class U>
	void writeTerminatorOfMyType(Address a)
	{
		File::i()->seek(a);
		writeTerminatorOfMyTypeHere();
	}

	/*
	This should fail if m_val.first is not an IType.
	*/
	template<class T, class U>
	void writeTerminatorOfMyTypeHere()
	{
		IType fst = dynamic_cast<IType>(m_val.first);
		fst.writeTerminatorOfMyTypeHere();
	}

	/*
	This should fail if m_val.first is not an IType.
	*/
	template<class T, class U>
	bool isZeroTerminator(Address a)
	{
		IType fst = dynamic_cast<IType>(m_val.first);
		return fst.isZeroTerminator(a);
	}

	/*
	Types of pair elements are supposed to inherit from IType.
	*/
	template<class T, class U>
	void read(Address a)
	{
		File::i()->seek(a);
		readHere();
	}

	/*
	Types of pair elements are supposed to inherit from IType.
	*/
	template<class T, class U>
	void write(Address a)
	{
		File::i()->seek(a);
		writeHere();
	}

	/*
	Types of pair elements are supposed to inherit from IType.
	*/
	template<class T, class U>
	void readHere()
	{
		IType fst = (IType)T();
		IType snd = (IType)U();

		fst.readHere();
		snd.readHere();

		m_val.first = fst;
		m_val.second = snd;
	}

	/*
	Types of pair elements are supposed to inherit from IType.
	*/
	template<class T, class U>
	void writeHere()
	{
		IType fst = (IType)m_val.first;
		IType snd = (IType)m_val.second;

		fst.writeHere();
		snd.writeHere();
	}

};