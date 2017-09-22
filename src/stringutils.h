#pragma once

#include "typedefs.h"

#include <vector>

/*
Returns an even-length vector of indices into the string, describing the words (left end inclusive, right end exclusive).
For example for string "xxx.xx...xx..." the following would be returned: [0, 3, 4, 6, 9, 11] 
                        01234567890123
*/

static std::vector<unsigned int> split_helper(String string, Char delim);
std::vector<String> split(String string, Char delim);

unsigned int StringDistance(String s1, String s2);