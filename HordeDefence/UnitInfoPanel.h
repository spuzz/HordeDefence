
#pragma once
#include <CEGUI/CEGUI.h>
#include "ImageButton.h"
#include "Unit.h"
#include <memory>

using std::shared_ptr;
class GUI;
class UnitInfoPanel
{
public:
	UnitInfoPanel(GUI* gui, const double& nWidthPerc, const double& nHeightPerc);
	~UnitInfoPanel();

	void setVisible(bool nVis);
	void updateInfo(shared_ptr<Unit> nUnit);

	

private:
	string UnitInfoPanel::convertTo2DecString(const float& nNumber);

	CEGUI::DefaultWindow* mNameTextInfo;
	CEGUI::DefaultWindow* mHealthTextInfo;
	CEGUI::DefaultWindow* mArmorTextInfo;
	CEGUI::DefaultWindow* mRangeTextInfo;
	CEGUI::DefaultWindow* mDamageTextInfo;
	CEGUI::DefaultWindow* mAttackSpeedTextInfo;

	shared_ptr<ImageButton> mHealth;
	shared_ptr<ImageButton> mArmor;
	shared_ptr<ImageButton> mRange;
	shared_ptr<ImageButton> mDamage;
	shared_ptr<ImageButton> mAttackSpeed;

	double mLineGapWidth;
	double mLineGapHeight;
	double mPictureHeight;
	double mPictureWidth;
	double mTextHeight;
	double mTextWidth;

};

