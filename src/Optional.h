#pragma once

template<class T>
class Optional
{
public:
	explicit operator bool() const { return m_some; }
	Optional();
	Optional(T t);
	bool Get(T& out);
private:
	bool m_some;
	T m_optional;
};