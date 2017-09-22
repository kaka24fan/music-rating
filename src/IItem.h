/*
Code written by Jakub (Kuba) Perlin in 2017.
*/

#pragma once

#include "stdafx.h"

class IItem
{
public:
	IItem(Id id);
	void assignAddress();

protected:
	Id m_id;
};