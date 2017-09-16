#include "Optional.h"

template <class T>
Optional<T>::Optional()
{
	m_some = false;
	m_optional = T{};
}

template<class T>
Optional<T>::Optional(T t)
{
	m_some = true;
	m_optional = t;
}

template<class T>
bool Optional<T>::Get(T& out)
{
	if (*this)
	{
		out = m_optional;
	}
	return (*this);
}
