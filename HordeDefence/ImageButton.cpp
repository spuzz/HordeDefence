#include "ImageButton.h"
#include "GUI.h"


ImageButton::ImageButton(CEGUI::Texture* text, CEGUI::BasicImage* image, CEGUI::PushButton* button, const string& imageName) : mText(text), mImage(image), mButton(button)
{
	const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), mText->getOriginalDataSize());
	mImage->setTexture(text);
	mImage->setArea(rect);
	mImage->setAutoScaled(CEGUI::ASM_Both);

	mButton->setProperty("NormalImage", imageName);
	mButton->setProperty("HoverImage", imageName);
	mButton->setProperty("PushedImage", imageName);
	mButton->setProperty("DisabledImage", imageName);
}

void ImageButton::changeButtonSize(const glm::vec2& destRectPerc, const glm::vec2& destRectPix)
{
	mButton->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
}

void ImageButton::changeTextureArea(const double& widthPerc, const double& heightPerc, CEGUI::Sizef pixelSize)
{
	CEGUI::Sizef startSize = mText->getOriginalDataSize();
	const CEGUI::Rectf rect(CEGUI::Vector2f(startSize.d_width  * widthPerc, startSize.d_height * heightPerc), pixelSize);
	mImage->setArea(rect);
}

void ImageButton::changeTextureRatio(const double& widthPerc, const double& heightPerc)
{
	CEGUI::Sizef size = mText->getOriginalDataSize();
	size.d_width = size.d_width * widthPerc;
	size.d_height = size.d_height * heightPerc;
	const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), size);
	mImage->setArea(rect);
}

void ImageButton::changeButtonImage(CEGUI::Texture* text)
{

	mImage->setTexture(text);

}

void ImageButton::setEnabled(const bool& enable)
{
	if (enable)
	{
		mButton->enable();
		mButton->setVisible(true);
	}
	else
	{
		mButton->disable();
		mButton->setVisible(false);
	}

}
ImageButton::~ImageButton()
{
}
