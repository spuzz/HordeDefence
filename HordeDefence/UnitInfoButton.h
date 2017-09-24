#pragma once
#include "ImageButton.h"
#include <memory>

using std::shared_ptr;
class UnitInfoButton
{
public:
	UnitInfoButton(shared_ptr<ImageButton> unitImage, shared_ptr<ImageButton> emptyHealthBar, shared_ptr<ImageButton> healthBar);
	~UnitInfoButton();

	shared_ptr<ImageButton> mUnitImage;
	shared_ptr<ImageButton> mEmptyHealthBar;
	shared_ptr<ImageButton> mHealthBar;

	void setEnabled(const bool& enable);

private:

};

