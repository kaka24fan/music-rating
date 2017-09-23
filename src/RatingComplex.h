/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

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
	// minor analogs of the ones from RatingSimple:
	void addNewMinorRating(TypeId mapId, TypeId minorId, Author author, TypeRatingData rating);
	void deleteMinorRating(TypeId mapId, TypeId minorId, Author author, unsigned int indexInVector);

	TypeRatingData getLastMinorRatingByAuthor(TypeId mapId, TypeId minorId, Author author);
	TypeVector<TypeRatingData> getMinorRatingsByAuthor(TypeId mapId, TypeId minorId, Author author);
	double getAverageMinorRatingFromAuthor(TypeId mapId, TypeId minorId, Author author);

	// inherited from RatingSimple:
	void addNewRating(TypeId mapId, Author author, TypeRatingData rating) override; // writes author,(val,emptyVec) into the map owned by this rating item

	void deleteRating(TypeId mapId, Author author, unsigned int indexInVector) override;

	TypeRatingData getLastRatingByAuthor(TypeId mapId, Author author) override;
	TypeVector<TypeRatingData> getRatingsByAuthor(TypeId mapId, Author author) override;
	TypeVector<Author> getAllAuthors(TypeId mapId) override;
	double getAverageRatingFromAuthor(TypeId mapId, Author author) override;

private:

};