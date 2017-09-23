/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"
#include "User.h"
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
	virtual void addNewRating(TypeId mapId, TypeId userId, TypeRatingData rating); // writes author,(val,emptyVec) into the map owned by this rating item
	
	virtual void deleteRating(TypeId mapId, TypeId userId, unsigned int indexInVector);
	
	virtual TypeRatingData getLastRatingByAuthor(TypeId mapId, TypeId userId);
	virtual TypeVector<TypeRatingData> getRatingsByAuthor(TypeId mapId, TypeId userId);
	virtual TypeVector<User> getAllUsers(TypeId mapId); // all users who authored at least one rating here
	virtual double getAverageRatingFromAuthor(TypeId mapId, TypeId userId);
};