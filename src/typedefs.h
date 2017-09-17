#pragma once

typedef unsigned char Byte; // 8 bits

//stores MinutesSince1970Epoch in the most significant 32 bits 
//and the rating in the least significant 6 bits (0, 0.5, 1, ..., 20 <-> 41 possibilities)
typedef uint64_t RatingVal; // 64 bits

typedef unsigned int Id; // 32 bits

typedef unsigned int Address; // 32 bits

typedef unsigned int PageIndex; // 32 bits

typedef char32_t Char; // 32 bits

typedef std::u32string Name;

/*
class RatingVal
{
public:
	RatingVal(unsigned char x);
	unsigned char x;

private:
};

class Byte
{
public:
	Byte(unsigned char x);
	unsigned char x;

private:
	
};

class Id
{
public:
	Id(unsigned short x);
	unsigned short x;

private:
	
};

class Char
{
public:
	Char(char32_t x);
	char32_t x;

private:
	
};

class Address
{
public:
	Address(unsigned int x);
	unsigned int x;

private:
	
};

class PageIndex
{
public:
	PageIndex(Address a);
	PageIndex(unsigned int a);
	unsigned int x;

private:
	
};

class Name
{
public:
	Name(std::string x);
	std::string str;

private:
};
*/