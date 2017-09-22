#pragma once

#include "stdafx.h"
#include "IComplexType.h"

/*
A zero-terminated sequential vector.
Storage pattern: e1, e2, e3, 0.
*/
template<class T>
class TypeVector
	: public IComplexType< std::vector<T> >
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
	template<class T>
	void read(Address a)
	{
		File::i()->seek(a);
		readHere();
	}

	/*
	Types of keys and values are supposed to inherit from IType.
	*/
	template<class T>
	void write(Address a)
	{
		File::i()->seek(a);
		writeHere();
	}

	/*
	Types of keys and values are supposed to inherit from IType.
	*/
	template<class T>
	void readHere()
	{
		while (true)
		{
			IType elem = (IType)T();

			Address tmp = File::i()->getNextBitToRW();
			if (elem.isZeroTerminator(tmp)) // terminator
			{
				break;
			}
			File::i()->seek(tmp);
			elem.readHere();

			m_val.push_back(elem);
		}
	}


	template<class T>
	void writeHere()
	{
		TypeId id(m_id);
		id.writeHere();
	}

	template<class T>
	void writeFull(Address a)
	{
		File::i()->seek(a);
		writeFullHere();
	}

	/*
	Types of keys and values are supposed to inherit from IType.
	*/
	template<class T>
	void writeFullHere()
	{
		IType& elem{};
		for (T t : m_val)
		{
			elem = dynamic_cast<IType&>(t);
			elem.writeHere();
		}
		elem.writeTerminatorOfMyTypeHere();
	}

	template<class T>
	void writeTerminatorOfMyType(Address a)
	{
		File::i()->seek(a);
		writeTerminatorOfMyTypeHere();
	}

	template<class T>
	void writeTerminatorOfMyTypeHere()
	{
		TypeId(42).writeTerminatorOfMyTypeHere(); // any TypeId will do because that function does not depend on state (could be static)
	}

	template<class T>
	bool isZeroTerminator(Address a)
	{
		TypeId testId = TypeId::constructFromAddress(a);
		return testId.isZeroTerminator();
	}

private:
};