#include "Offhand.h"


Offhand::Offhand(string type, string name)
{
	setType(type);
	if (name.compare("")) {
		setName(type);
	}
	else {
		setName(name);
	}
}


Offhand::~Offhand()
{
}
