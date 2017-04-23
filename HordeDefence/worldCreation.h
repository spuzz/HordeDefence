#pragma once

#include "Model.h"
#include "View.h"
#include <memory>
#include "GameMath.h"


using std::shared_ptr;
class worldCreation
{
public:
	worldCreation(Model* inModel);
	virtual ~worldCreation();
};

