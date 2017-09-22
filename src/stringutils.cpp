/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "stringutils.h"

#include "Assert.h"

std::vector<unsigned int> split_helper(String string, Char delim)
{
	std::vector<unsigned int> result = std::vector<unsigned int>{};
	bool wordStarted = false;
	for (unsigned int i = 0; i < string.length(); i++)
	{
		if ((string.at(i) != delim) ^ wordStarted)
		{
			wordStarted = !wordStarted;
			result.push_back(i);
		}
	}
	if (wordStarted)
		result.push_back((unsigned int)string.length());

	assert(result.size() % 2 == 0);
	return result;
}

std::vector<String> split(String string, Char delim)
{
	std::vector<String> result{};
	std::vector<unsigned int> indices = split_helper(string, delim);
	for (unsigned int i = 0; i < indices.size(); i += 2)
	{
		String s{};
		for (unsigned int j = indices.at(i); j < indices.at(i + 1); j++)
		{
			s.push_back(string.at(j));
		}
		result.push_back(s);
	}
	return result;
}

unsigned int StringDistance(String s1, String s2)
{
	if (s1.length() == s2.length())
	{
		unsigned int dist = 0;
		for (unsigned int i = 0; i < s1.length(); i++)
		{
			if (s1.at(i) != s2.at(i))
				dist++;
		}
		return dist;
	}
	return 6969;
}