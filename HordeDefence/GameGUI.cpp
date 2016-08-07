#include "GameGUI.h"
#include "Control.h"

GameGUI::GameGUI()
{
}

void GameGUI::loadGUI()
{
	init("../HordeDefenceArt/GUI");
	loadScheme("OgreTray.scheme");
	setFont("DejaVuSans-10");
	loadGameGUI();
	showMouseCursor();
}

void GameGUI::loadGameGUI()
{
	m_bBlockInput = false;
	CEGUI::FrameWindow* bottomFrame = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/FrameWindow",
		glm::vec4(0.0f, 0.85f, 1.0f, 0.15f), glm::vec4(0.0f), "bottomFrame"));
	m_bMenuUp = false;

}

bool GameGUI::mouseAction(int button, int action)
{
	injectMouseAction(button, action);
	return true;
}

bool GameGUI::keyAction(int key, int scancode, int action)
{
	if (m_bBlockInput)
	{
		return false;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		if (toggleGameMenu(2))
		{
			m_Control->setPause(true);
		}
		else
		{
			m_Control->setPause(false);
		}

}

/*  n_bActivate
0 = only close
1 = only open
2 = toggle
*/
bool GameGUI::toggleGameMenu(int n_bActivate)
{
	if (m_bMenuUp == false && n_bActivate != 0)
	{
		m_bBlockInput = true;
		CEGUI::FrameWindow* background = (CEGUI::FrameWindow*)createWidget("OgreTray/AltStaticImage",
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f), "background");
		background->setProperty(background->RiseOnClickEnabledPropertyName, "false");
		CEGUI::FrameWindow* menuFrame = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/FrameWindow",
			glm::vec4(0.4f, 0.3f, 0.2f, 0.34f), glm::vec4(0.0f), "menuFrame"));

		CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.32f, 0.18f, 0.05f), glm::vec4(0.0f), "Return"));
		button->setText("Return to Game");

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.37f, 0.18f, 0.05f), glm::vec4(0.0f), "Settings"));
		button->setText("Settings");

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.42f, 0.18f, 0.05f), glm::vec4(0.0f), "Load"));
		button->setText("Load Game");

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.47f, 0.18f, 0.05f), glm::vec4(0.0f), "Save"));
		button->setText("Save Game");

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.52f, 0.18f, 0.05f), glm::vec4(0.0f), "QuitToMain"));
		button->setText("Quit To Main Menu");

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.57f, 0.18f, 0.05f), glm::vec4(0.0f), "QuitToDesktop"));
		button->setText("Quit To Desktop");
		button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameGUI::onExitToDesktop, this));

		m_bMenuUp = true;
	}
	else if (n_bActivate != 1) {
		m_bBlockInput = false;
		destroyWidget("menuFrame");
		destroyWidget("Return");
		destroyWidget("Settings");
		destroyWidget("Load");
		destroyWidget("Save");
		destroyWidget("QuitToMain");
		destroyWidget("QuitToDesktop");
		destroyWidget("background");
		m_bMenuUp = false;
	}

	return m_bMenuUp;
}

bool GameGUI::onExitToDesktop(const CEGUI::EventArgs& e)
{
	CEGUI::FrameWindow* background = (CEGUI::FrameWindow*)createWidget("OgreTray/AltStaticImage",
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f), "popupbackground");
	background->setProperty(background->RiseOnClickEnabledPropertyName, "false");
	CEGUI::FrameWindow* popupMenu = (CEGUI::FrameWindow*)createWidget("OgreTray/StaticImage",
		glm::vec4(0.4f, 0.3f, 0.2f, 0.15f), glm::vec4(0.0f), "popup");
	CEGUI::Window* statictext = (CEGUI::Window*)createWidget("OgreTray/StaticText",
		glm::vec4(0.41f, 0.31f, 0.18f, 0.05f), glm::vec4(0.0f), "text");
	statictext->setText("Really exit to desktop?");
	statictext->setProperty("FrameEnabled", "false");
	statictext->setProperty("BackgroundEnabled", "false");
	CEGUI::PushButton* quit = (CEGUI::PushButton*)createWidget("OgreTray/Button",
		glm::vec4(0.41f, 0.38f, 0.08f, 0.05f), glm::vec4(0.0f), "quit");
	quit->setText("Quit");
	quit->subscribeEvent("Clicked", CEGUI::Event::Subscriber(&Control::exitToDesktop, &(*m_Control)));
	CEGUI::PushButton* cancel = (CEGUI::PushButton*)createWidget("OgreTray/Button",
		glm::vec4(0.51f, 0.38f, 0.08f, 0.05f), glm::vec4(0.0f), "cancel");
	cancel->setText("Cancel");
	cancel->subscribeEvent("Clicked", CEGUI::Event::Subscriber(&GameGUI::quitCancel, this));
	return true;
}

bool GameGUI::quitCancel(const CEGUI::EventArgs& e)
{
	destroyWidget("popupbackground");
	destroyWidget("popup");
	destroyWidget("quit");
	destroyWidget("cancel");
	destroyWidget("text");
	return true;
}

GameGUI::~GameGUI()
{
}
