#include "GameGUI.h"
#include "Control.h"

bool comparator(const std::shared_ptr<Sprite> &a, const std::shared_ptr<Sprite> &b) {
	return (a->getIsoDepth() < b->getIsoDepth());
}


GameGUI::GameGUI()
{
	m_zoomFactor = 9;
	m_xScreenLoc = 0;
	m_yScreenLoc = -40;
	m_zScreenLoc = -220;
	m_viewZoomFactor = 55.0f;
	m_yIncreasing = 0;
	m_xIncreasing = 0;
}

void GameGUI::start()
{
	setupGL();
}

void GameGUI::update()
{
	changeXScreenLoc(m_xIncreasing);
	changeYScreenLoc(m_yIncreasing);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	GLuint currentTxtr = 0;
	std::vector<std::shared_ptr<Sprite>>::iterator& it = m_model->getSprites().begin();
	std::vector<std::shared_ptr<Sprite>>& sprites = m_model->getSprites();
	TopologicalGraphSort(sprites);
	std::sort(sprites.begin(), sprites.end(), comparator);
	int count = 0;
	for (it = sprites.begin(); it != sprites.end(); ++it)
	{
		if (count == 0)
		{
			count = 1;
		}
		else {
			(*it)->draw(m_xScreenLoc, m_yScreenLoc, m_zScreenLoc, *m_txtrLoader.get());
		}
	}
	draw();
	glLoadIdentity();

	glfwSwapBuffers(m_window.get());
	glfwPollEvents();
}

void GameGUI::close()
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

	if (checkScreenMoveKey(key, action))
	{
		return true;
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
		menuFrame->setDisabled(true);

		CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.32f, 0.18f, 0.05f), glm::vec4(0.0f), "Return"));
		button->setText("Return to Game");
		button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameGUI::onReturnToGame, this));

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

bool GameGUI::onReturnToGame(const CEGUI::EventArgs& e)
{
	toggleGameMenu(false);
	return true;
}

bool GameGUI::onExitToDesktop(const CEGUI::EventArgs& e)
{
	CEGUI::FrameWindow* background = (CEGUI::FrameWindow*)createWidget("OgreTray/AltStaticImage",
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f), "popupbackground");
	background->setProperty(background->RiseOnClickEnabledPropertyName, "false");
	CEGUI::FrameWindow* popupMenu = (CEGUI::FrameWindow*)createWidget("OgreTray/StaticImage",
		glm::vec4(0.4f, 0.3f, 0.2f, 0.15f), glm::vec4(0.0f), "popup");
	popupMenu->setDisabled(true);
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

void GameGUI::setupGL()
{
	float ratio;
	glfwGetFramebufferSize(m_window.get(), &m_width, &m_height);

	perspectiveSetup();
	ratio = m_width / (float)m_height;
	InitGL();
}

void GameGUI::perspectiveSetup() {
	float ratio;
	ratio = m_width / (float)m_height;
	glViewport(0, 0, m_width, m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width / 10, width / 10, -height / 10, height / 10, -350, 350);
	perspectiveGL(m_viewZoomFactor / m_zoomFactor, (GLfloat)m_width / (GLfloat)m_height, 0.1f, 350.0f);
	glMatrixMode(GL_MODELVIEW);
}

void GameGUI::InitGL()
{

	//CEGUI::OpenGL3Renderer& gui = CEGUI::OpenGL3Renderer::bootstrapSystem();
	m_texture.push_back(m_txtrLoader->retrieveMapTexture()[0]);
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable(GL_LIGHT0);								// Quick And Dirty Lighting (Assumes Light0 Is Set Up)
	glEnable(GL_LIGHTING);								// Enable Lighting
	glEnable(GL_COLOR_MATERIAL);						// Enable Material Coloring
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

}

void GameGUI::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

// Determine dependencies for the topological graph sort
void GameGUI::TopologicalGraphSort(std::vector<std::shared_ptr<Sprite>>& nSprites)
{
	std::vector<std::shared_ptr<Sprite>> tmpSprites;
	int test = 0;
	for (int a = 0; a < nSprites.size(); a++)
	{

		nSprites[a]->clearSpriteBehind();
		if (nSprites[a]->getIsoLocation().getZ() >= 2)
		{
			tmpSprites.push_back(std::shared_ptr<Sprite>(nSprites[a]));

		}
		else
		{
			nSprites[a]->setIsoDepth(0);
		}

	}
	int behindIndex;
	const int isoSpritesLength = tmpSprites.size();
	for (int i = 0; i < isoSpritesLength; ++i)
	{

		Sprite& a = *tmpSprites[i];
		behindIndex = 0;

		for (int j = 0; j < isoSpritesLength; ++j)
		{
			if (i != j)
			{
				Sprite& b = *tmpSprites[j];

				if (b.getIsoLocation().getX() == 41 && b.getIsoLocation().getY() == 63)
				{
					int trap = 1;
				}
				if (b.getMin().getX() < a.getMax().getX() && b.getMin().getY() < a.getMax().getY() && b.getMin().getZ() <= a.getMax().getZ())
				{
					if (b.getMin().getZ() == a.getMax().getZ() && (b.getMax().getY() > a.getMax().getX()))
						continue;
					a.addSpriteBehind(tmpSprites[j]);
				}
			}
		}

		a.setIsoVisitedFlag(false);
	}
	int depthTest = 1;
	for (int i = 0; i < tmpSprites.size(); ++i)
	{
		tmpSprites[i]->visitNode(depthTest);
	}

}

void GameGUI::increaseZoomFactor(float inZoomIncrease)
{
	if ((m_zoomFactor + inZoomIncrease) >= 6 && (m_zoomFactor + inZoomIncrease) <= 20) {
		m_zoomFactor += inZoomIncrease;
	}
	perspectiveSetup();

}
void GameGUI::decreaseZoomFactor(float inZoomDecrease)
{
	if ((m_zoomFactor + inZoomDecrease) >= 6 && (m_zoomFactor + inZoomDecrease) <= 20) {
		m_zoomFactor += inZoomDecrease;
	}
	perspectiveSetup();
}

void GameGUI::changeXScreenLoc(float inXScreenLoc)
{
	if ((m_xScreenLoc + inXScreenLoc) > -50 && (m_xScreenLoc + inXScreenLoc) < 50)
	{
		m_xScreenLoc += inXScreenLoc;
	}

}
void GameGUI::changeYScreenLoc(float inYScreenLoc)
{
	if ((m_yScreenLoc + inYScreenLoc) < -20 && (m_yScreenLoc + inYScreenLoc) > -80)
	{
		m_yScreenLoc += inYScreenLoc;
	}
}

bool GameGUI::checkScreenMoveKey(int key, int action)
{
	// xMove = 0 yMove = 1
	int xyMovement = -1;
	int direction = 0;
	switch (key)
	{
	case GLFW_KEY_A:
		xyMovement = 0;
		direction = 1;
		break;
	case GLFW_KEY_D:
		xyMovement = 0;
		direction = -1;
		break;
	case GLFW_KEY_W:
		xyMovement = 1;
		direction = -1;
		break;
	case GLFW_KEY_S:
		xyMovement = 1;
		direction = 1;
		break;
	default:
		return false;
	}
	if (action == GLFW_PRESS)
	{
		if (xyMovement)
		{
			setYMovement(direction);
		}
		else
		{
			setXMovement(direction);
		}
	}
	else if (action == GLFW_RELEASE)
	{
		if (xyMovement)
		{
			if (getYMovement() != -(direction)) {
				setYMovement(0);
			}
		}
		else
		{
			if (getXMovement() != -(direction)) {
				setXMovement(0);
			}
		}
	}
	return true;
}

GameGUI::~GameGUI()
{
}
