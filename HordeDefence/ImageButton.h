#pragma once
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>
#include <GL\glew.h>
#include <GLFW/glfw3.h>

using std::string;

class ImageButton
{
public:
	ImageButton(CEGUI::Texture* text, CEGUI::BasicImage* image, CEGUI::PushButton* button, const string& imageName);
	virtual ~ImageButton();

	void changeButtonSize(const glm::vec2& destRectPerc, const glm::vec2& destRectPix);
	void changeTextureRatio(const double& widthPerc, const double& heightPerc);
	void changeTextureArea(const double& widthPerc, const double& heightPerc, CEGUI::Sizef pixelSize );
	void changeButtonImage(CEGUI::Texture* text);
	void setEnabled(const bool& enable);
private:
	CEGUI::Texture* mText;
	CEGUI::BasicImage* mImage;
	CEGUI::PushButton* mButton;
};

