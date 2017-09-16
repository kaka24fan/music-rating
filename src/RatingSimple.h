#pragma once

#include "stdafx.h"

static const int MIN_RATING = 1;
static const int MAX_RATING = 20;

/*
A SimpleRating is identified with a map:
map<Author, vector<RatingVal>>.
Thus it is not an item in its own right as opposed to Authors etc.

For example, a simple rating will be used to rate an album,
as an album is just one thing to rate with additional pointers
to independently rated things (songs).
The vectorness represents change over time.

The following is just an interface to operate on these SimpleRating-maps.
The map's mapId must be an argument in all interface methods.
*/
class RatingSimple
{
public:
	//void addNewRating(TypeId mapId, Author author, TypeRatingVal rating); // writes author,(val,emptyVec) into the map owned by this rating item
	//TypeVector<TypeRatingVal> getRatingsOfAuthor(TypeId mapId, Author author);
	//TypeVector<Author> getAllAuthors(TypeId mapId);
	//double getAverageRatingFromAuthor(TypeId mapId, Author author);
};