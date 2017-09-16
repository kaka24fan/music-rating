#pragma once

#include "stdafx.h"

template <class T, class U>
void TypePair<T, U>::read(Address a)
{
	File::i()->seek(a);
	readHere();
}

template <class T, class U>
void TypePair<T, U>::write(Address a)
{
	File::i()->seek(a);
	writeHere();
}

/*
This will hopefully fail if T or U does not inherit from IType?
*/
template <class T, class U>
void TypePair<T, U>::readHere()
{
	IType fst = (IType)T();
	IType snd = (IType)U();

	fst.readHere();
	snd.readHere();

	m_val.first = fst;
	m_val.second = snd;
}

/*
This will hopefully fail if T or U does not inherit from IType?
*/
template <class T, class U>
void TypePair<T, U>::writeHere()
{
	IType fst = (IType)m_val.first;
	IType snd = (IType)m_val.second;

	fst.writeHere();
	snd.writeHere();
}