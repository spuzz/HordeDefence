#pragma once
#include "Item.h"
class Weapon :
	public Item
{
public:
	Weapon() { setType("sword"), setName("sword"); }
	Weapon(string inType,string name = "");
	virtual ~Weapon();
};

