#pragma once

#include "Model.h"
#include "View.h"
#include <memory>

using std::shared_ptr;
class worldCreation
{
public:
	worldCreation(shared_ptr<Model> inModel, shared_ptr<View> inView);
	virtual ~worldCreation();
};

