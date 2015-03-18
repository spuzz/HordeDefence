#include "Armor.h"


Armor::Armor(string type, string name)
{
	setType(type);
	if (name.compare("")) {
		setName(type);
	}
	else {
		setName(name);
	}
}


Armor::~Armor()
{
}
