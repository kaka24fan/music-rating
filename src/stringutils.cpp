/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#include "stringutils.h"

#include "Assert.h"
#include <map>

/*
These values were obtained using the code:
	(unsigned int) lineOfInput.at(0)
and the input stream std::wcin
*/
static std::map<Char, Char> capitalPolishToLowerPolish =
{
	{ (Char)164,			(Char)165 }, // r_a_czka
	{ (Char)143,			(Char)134 }, // jeba_c_		
	{ (Char)168,			(Char)169 }, // r_e_ka		
	{ (Char)157,			(Char)136 }, // zo_l_w	
	{ (Char)227,			(Char)228 }, // ko_n_		
	{ (Char)224,			(Char)162 }, // g_o_wno		
	{ (Char)151,			(Char)152 }, // _s_nieg		
	{ (Char)141,			(Char)171 }, // _z_rebak	
	{ (Char)189,			(Char)190 }, // _z_olw		
};

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

Char toLower(Char c)
{
	if ('A' <= c && c <= 'Z')
	{
		return (c - 'A') + 'a';
	}

	for (std::pair<Char, Char> kvp : capitalPolishToLowerPolish)
	{
		if (c == kvp.first)
			return kvp.second;
	}

	// didn't find anything
	return c;
}

unsigned int StringDistance(String s1, String s2)
{
	if (s1.length() == s2.length())
	{
		unsigned int dist = 0;
		for (unsigned int i = 0; i < s1.length(); i++)
		{
			if (toLower(s1.at(i)) != toLower(s2.at(i)))
				dist++;
		}
		return dist;
	}
	return 6969;
}