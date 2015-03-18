#pragma once
#include "Item.h"
class Armor :
	public Item
{
public:
	Armor() { setType("clothes"),setName("clothes"); }
	Armor(string type, string name = "");
	virtual ~Armor();
};

