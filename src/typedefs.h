/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include <string>
#include <istream>
#include <ostream>

typedef unsigned char Byte; // 8 bits

//stores MinutesSince1970Epoch in the most significant 32 bits 
//and the rating in the least significant 6 bits (0, 0.5, 1, ..., 20 <-> 41 possibilities)
typedef uint64_t RatingData; // 64 bits

typedef unsigned int Id; // 32 bits

typedef unsigned int Address; // 32 bits

typedef unsigned int PageIndex; // 32 bits

typedef wchar_t Char; // implementation specific - 16/32 bits?

typedef std::basic_string<Char> String;

typedef String Name;

//typedef std::basic_istream<Char, std::char_traits<Char> > Cin;
//typedef std::basic_ostream<Char, std::char_traits<Char> > Cout;
