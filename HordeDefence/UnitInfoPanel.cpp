#include "UnitInfoPanel.h"
#include "GUI.h"

#include <iomanip> // setprecision

UnitInfoPanel::UnitInfoPanel(GUI* gui, const double& nWidthPerc, const double& nHeightPerc)
{
	CEGUI::Font& fnt = CEGUI::FontManager::getSingleton().createFromFile("mizufalp-8.font");
	mLineGapWidth = 0.05;
	mLineGapHeight = 0.04;
	mPictureWidth = 0.03;
	mPictureHeight = 0.03;
	mTextHeight = 0.02;
	mTextWidth = 0.08;

	mNameTextInfo = (CEGUI::DefaultWindow*)gui->createWidget("OgreTray/StaticText",
		glm::vec4(nWidthPerc + (mLineGapWidth * 2), nHeightPerc, mTextWidth, mTextHeight), glm::vec4(0.0f), "NameInfo");

	mHealthTextInfo = (CEGUI::DefaultWindow*)gui->createWidget("OgreTray/StaticText",
		glm::vec4(nWidthPerc + (mLineGapWidth * 2), nHeightPerc + mLineGapHeight, mTextWidth, mTextHeight), glm::vec4(0.0f), "HealthInfo");
	mHealthTextInfo->setText("Health");
	mHealthTextInfo->setFont(&fnt);
	mArmorTextInfo = (CEGUI::DefaultWindow*)gui->createWidget("OgreTray/StaticText",
		glm::vec4(nWidthPerc + mLineGapWidth, nHeightPerc + (mLineGapHeight*2), mTextWidth, mTextHeight), glm::vec4(0.0f), "ArmorInfo");
	mArmorTextInfo->setText("Armor");
	mArmorTextInfo->setFont(&fnt);
	mRangeTextInfo = (CEGUI::DefaultWindow*)gui->createWidget("OgreTray/StaticText",
		glm::vec4(nWidthPerc + (mLineGapWidth*3), nHeightPerc + (mLineGapHeight * 2), mTextWidth, mTextHeight), glm::vec4(0.0f), "RangeInfo");
	mRangeTextInfo->setText("Range");
	mRangeTextInfo->setFont(&fnt);
	mDamageTextInfo = (CEGUI::DefaultWindow*)gui->createWidget("OgreTray/StaticText",
		glm::vec4(nWidthPerc + mLineGapWidth, nHeightPerc + (mLineGapHeight * 3), mTextWidth, mTextHeight), glm::vec4(0.0f), "DamageInfo");
	mDamageTextInfo->setText("Damage");
	mDamageTextInfo->setFont(&fnt);
	mAttackSpeedTextInfo = (CEGUI::DefaultWindow*)gui->createWidget("OgreTray/StaticText",
		glm::vec4(nWidthPerc + (mLineGapWidth*3), nHeightPerc + (mLineGapHeight * 3), mTextWidth, mTextHeight), glm::vec4(0.0f), "AttackSpeedInfo");
	mAttackSpeedTextInfo->setText("Attack Speed");
	mAttackSpeedTextInfo->setFont(&fnt);

	mHealth = gui->createImageButton(glm::vec4(nWidthPerc + mLineGapWidth, nHeightPerc + mLineGapHeight, mPictureWidth, mPictureHeight), "HealthIcon", "Icons");
	mHealth->changeTextureArea(0.25, ((100.0/26.0)*23)/100, CEGUI::Sizef(16,16));
	mArmor = gui->createImageButton(glm::vec4(nWidthPerc, nHeightPerc + (mLineGapHeight * 2), mPictureWidth, mPictureHeight), "ArmorIcon", "Icons");
	mArmor->changeTextureArea(0.4375, ((100.0 / 26.0) * 23) / 100, CEGUI::Sizef(16, 16));
	mRange = gui->createImageButton(glm::vec4(nWidthPerc + (mLineGapWidth * 2), nHeightPerc + (mLineGapHeight * 2), mPictureWidth, mPictureHeight), "RangeIcon", "Icons");
	mRange->changeTextureArea(0.375, ((100.0 / 26.0) * 9) / 100, CEGUI::Sizef(32, 32));
    mDamage = gui->createImageButton(glm::vec4(nWidthPerc, nHeightPerc + (mLineGapHeight * 3), mPictureWidth, mPictureHeight), "DamageIcon", "Icons");
	mDamage->changeTextureArea(0.375, ((100.0 / 26.0) * 23) / 100, CEGUI::Sizef(16, 16));
	mAttackSpeed = gui->createImageButton(glm::vec4(nWidthPerc + (mLineGapWidth * 2), nHeightPerc + (mLineGapHeight * 3), mPictureWidth, mPictureHeight), "AttackSpeedIcon", "Icons");
	mAttackSpeed->changeTextureArea(0.625, ((100.0 / 26.0) * 23) / 100, CEGUI::Sizef(16, 16));
}

void UnitInfoPanel::setVisible(bool nVis)
{
	mNameTextInfo->setVisible(nVis);
	mHealthTextInfo->setVisible(nVis);
	mArmorTextInfo->setVisible(nVis);
	mRangeTextInfo->setVisible(nVis);
	mDamageTextInfo->setVisible(nVis);
	mAttackSpeedTextInfo->setVisible(nVis);

	mHealth->setEnabled(nVis);
	mArmor->setEnabled(nVis);
	mRange->setEnabled(nVis);
	mDamage->setEnabled(nVis);
	mAttackSpeed->setEnabled(nVis);
}

void UnitInfoPanel::updateInfo(shared_ptr<Unit> nUnit)
{
	string health = std::to_string(static_cast<int>(nUnit->getCurrentHealth())) + " / " + std::to_string(static_cast<int>(nUnit->getMaxHealth()));
	mHealthTextInfo->setText(health);
	mArmorTextInfo->setText(std::to_string(0));
	mRangeTextInfo->setText(std::to_string(static_cast<int>(nUnit->getRange())));
	mDamageTextInfo->setText(std::to_string(30));
	mAttackSpeedTextInfo->setText(convertTo2DecString(nUnit->getAttackSpeed()));
}

string UnitInfoPanel::convertTo2DecString(const float& nNumber)
{
	std::stringstream stream;
	stream << std::setprecision(2) << nNumber;
	return stream.str();
}

UnitInfoPanel::~UnitInfoPanel()
{
}
