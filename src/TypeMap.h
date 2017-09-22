/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "IComplexType.h"

/*
A zero-terminated sequential map.
Storage pattern: k1, v1, k2, v2, k3, v3, 0.
*/
template<class T, class U>
class TypeMap 
	: public IComplexType< std::map<T,U> >
{
public:
	void read(Address a) override;
	void write(Address a) override;
	void readHere() override;
	void writeHere() override;
	void writeFull(Address a) override;
	void writeFullHere() override;
	void writeTerminatorOfMyType(Address a) override;
	void writeTerminatorOfMyTypeHere() override;
	bool isZeroTerminator(Address a) override;

	/*
	Types of keys and values are supposed to inherit from IType.
	*/
	template<class T, class U>
	void read(Address a)
	{
		File::i()->seek(a);
		readHere();
	}

	/*
	Types of keys and values are supposed to inherit from IType.
	*/
	template<class T, class U>
	void write(Address a)
	{
		File::i()->seek(a);
		writeHere();
	}

	/*
	Types of keys and values are supposed to inherit from IType.
	*/
	template<class T, class U>
	void readHere()
	{
		while (true)
		{
			IType key = (IType)T();
			IType val = (IType)U();

			Address tmp = File::i()->getNextBitToRW();
			if (key.isZeroTerminator(tmp)) // terminator
			{
				break;
			}
			File::i()->seek(tmp);
			key.readHere();
			val.readHere();

			m_val.insert(std::pair<T, U>((T)key, (U)val));
		}
	}

	template<class T, class U>
	void writeHere()
	{
		TypeId id(m_id);
		id.writeHere();
	}

	template<class T, class U>
	void writeFull(Address a)
	{
		File::i()->seek(a);
		writeFullHere();
	}

	/*
	Types of keys and values are supposed to inherit from IType.
	*/
	template<class T, class U>
	void writeFullHere()
	{
		IType& fst = IType{};
		for (std::pair<T, U> kvp : m_val)
		{
			fst = dynamic_cast<IType&>(kvp.first);
			IType snd = dynamic_cast<IType&>(kvp.second);
			fst.writeHere();
			snd.writeHere();
		}
		fst.writeTerminatorOfMyTypeHere();
	}

	template<class T, class U>
	void writeTerminatorOfMyType(Address a)
	{
		File::i()->seek(a);
		writeTerminatorOfMyTypeHere();
	}

	template<class T, class U>
	void writeTerminatorOfMyTypeHere()
	{
		TypeId(42).writeTerminatorOfMyTypeHere(); // any TypeId will do because that function does not depend on state (could be static)
	}

	template<class T, class U>
	bool isZeroTerminator(Address a)
	{
		TypeId testId;
		testId.constructFromAddress(a);
		return testId.isZeroTerminator();
	}

private:
};