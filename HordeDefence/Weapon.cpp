#include "Weapon.h"


Weapon::Weapon(string type, string name)
{
	setType(type);
	if (name.compare("")) {
		setName(type);
	}
	else {
		setName(name);
	}
}


Weapon::~Weapon()
{
}
