#pragma once

#include "Model.h"
#include <memory>

using std::shared_ptr;
class worldCreation
{
public:
	worldCreation(shared_ptr<Model> inModel);
	virtual ~worldCreation();
};

