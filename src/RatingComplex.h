#pragma once

#include "stdafx.h"
#include "RatingSimple.h"

/*
A ComplexRating is identified with a map:
map<Author, pair<vector<RatingVal>, map<Id, vector<RatingVal>>>.
Thus it is not an item in its own right as opposed to Authors etc.

The complex rating is used for songs:
The inner map maps flagged IDs such as zeus-producer id and zeus-singer id 
to their vector-ratings (repping change over time).

SimpleRating is logically strictly contained in ComplexRating, thus
the interface for the latter will inherit from the former and override all methods.

The following is just an interface to operate on these SimpleRating-maps.
The map's mapId must be an argument in all interface methods.
*/
class RatingComplex : public RatingSimple
{
public:
	void addNewMinorRating(TypeId mapId, Author author, TypeId minorId, TypeRatingData rating);
	// analogs of the ones below...

	// inherited from RatingSimple:
	void addNewRating(TypeId mapId, Author author, TypeRatingData rating) override; // writes author,(val,emptyVec) into the map owned by this rating item
	TypeVector<TypeRatingData> getRatingsOfAuthor(TypeId mapId, Author author) override;
	TypeVector<Author> getAllAuthors(TypeId mapId) override;
	double getAverageRatingFromAuthor(TypeId mapId, Author author) override;
	void deleteRating(TypeId mapId, Author author, unsigned int indexInVector) override;

private:

};