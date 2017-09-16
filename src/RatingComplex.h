#pragma once

#include "stdafx.h"

/*
A ComplexRating is identified with a map:
map<Author, pair<vector<RatingVal>, map<Id, vector<RatingVal>>>.
Thus it is not an item in its own right as opposed to Authors etc.

The complex rating is used for songs:
The inner map maps flagged IDs such as zeus-producer id and zeus-singer id 
to their vector-ratings (repping change over time).

The following is just an interface to operate on these SimpleRating-maps.
The map's mapId must be an argument in all interface methods.
*/
class RatingComplex
{
public:
	
};