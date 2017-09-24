#include "UnitInfoButton.h"




UnitInfoButton::UnitInfoButton(shared_ptr<ImageButton> unitImage, shared_ptr<ImageButton> emptyHealthBar, shared_ptr<ImageButton> healthBar) : mUnitImage(unitImage), mEmptyHealthBar(emptyHealthBar), mHealthBar(healthBar)
{
}


UnitInfoButton::~UnitInfoButton()
{
}

void UnitInfoButton::setEnabled(const bool& enable)
{
	mUnitImage->setEnabled(enable);
	mEmptyHealthBar->setEnabled(enable);
	mHealthBar->setEnabled(enable);
}