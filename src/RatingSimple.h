/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "Author.h"
#include "TypeRatingData.h"
#include "TypeVector.h"

/*
A SimpleRating is identified with a map:
map<Author, vector<RatingVal>>.
Thus it is not an item in its own right as opposed to Authors etc.

For example, a simple rating will be used to rate an album,
as an album is just one thing to rate with additional pointers
to independently rated things (songs).
The vectorness represents change over time.

SimpleRating is logically strictly contained in ComplexRating, thus
the interface for the latter will inherit from here and override all methods.

The following is just an interface to operate on these SimpleRating-maps.
The map's mapId must be an argument in all interface methods.
*/
class RatingSimple
{
public:
	virtual void addNewRating(TypeId mapId, Author author, TypeRatingData rating); // writes author,(val,emptyVec) into the map owned by this rating item
	virtual TypeVector<TypeRatingData> getRatingsOfAuthor(TypeId mapId, Author author);
	virtual TypeVector<Author> getAllAuthors(TypeId mapId);
	virtual double getAverageRatingFromAuthor(TypeId mapId, Author author);
	virtual void deleteRating(TypeId mapId, Author author, unsigned int indexInVector);
};