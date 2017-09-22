/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "Assert.h"

void assert(bool b)
{
	if (!b)
	{
#ifdef MY_DEBUG
		__debugbreak();
#endif
		exit(-1);
	}
}