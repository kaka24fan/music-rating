/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"

class TypeId;

/*
An interface for items such as: Vector, Map, Album, Artist, Song, User, SimpleRating, ComplexRating. 
*/
class IItem
{
public:
	IItem() = delete;
	static void assignAddress(TypeId thisItemId);

private:
};