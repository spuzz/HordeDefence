#pragma once
#include "Item.h"
class Offhand :
	public Item
{
public:
	Offhand() { setType("buckler"), setName("buckler"); }
	Offhand(string type, string name = "");
	virtual ~Offhand();
};

