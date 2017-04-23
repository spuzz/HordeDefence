#include "MainMenuGUI.h"
#include "Control.h"


MainMenuGUI::MainMenuGUI()
{
}


MainMenuGUI::~MainMenuGUI()
{
	destroy();
}

void MainMenuGUI::loadGUI()
{
	init("../HordeDefenceArt/GUI");
	loadScheme("OgreTray.scheme");
	setFont("DejaVuSans-10");

	CEGUI::FrameWindow* background = (CEGUI::FrameWindow*)createWidget("OgreTray/AltStaticImage",
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f), "background");
	background->setProperty(background->RiseOnClickEnabledPropertyName, "false");
	CEGUI::FrameWindow* menuFrame = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/FrameWindow",
		glm::vec4(0.4f, 0.3f, 0.2f, 0.24f), glm::vec4(0.0f), "menuFrame"));
	menuFrame->setDisabled(true);

	CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.41f, 0.32f, 0.18f, 0.05f), glm::vec4(0.0f), "NewGame"));
	button->setText("New Game");
	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Control::NewGame, &(*m_Control)));

	button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.41f, 0.37f, 0.18f, 0.05f), glm::vec4(0.0f), "Settings"));
	button->setText("Settings");
	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Control::Settings, &(*m_Control)));

	button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.41f, 0.42f, 0.18f, 0.05f), glm::vec4(0.0f), "Load"));
	button->setText("Load Game");

	button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.41f, 0.47f, 0.18f, 0.05f), glm::vec4(0.0f), "QuitToDesktop"));
	button->setText("Quit To Desktop");
	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuGUI::onExitToDesktop, this));
}

bool MainMenuGUI::onNewGame(const CEGUI::EventArgs& e)
{
	return true;
}

bool MainMenuGUI::onExitToDesktop(const CEGUI::EventArgs& e)
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
	return true;
}

bool MainMenuGUI::mouseAction(int button, int action)
{
	injectMouseAction(button, action);
	return true;
}

bool MainMenuGUI::keyAction(int key, int scancode, int action)
{
	return false;
}

void MainMenuGUI::start()
{
	setupGL();
}

void MainMenuGUI::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	draw();
	glLoadIdentity();

	glfwSwapBuffers(m_window.get());
	glfwPollEvents();
}

void MainMenuGUI::setupGL()
{
	float ratio;
	glfwGetFramebufferSize(m_window.get(), &m_width, &m_height);

	perspectiveSetup();
	ratio = m_width / (float)m_height;
	InitGL();
}

void MainMenuGUI::perspectiveSetup() {
	float ratio;
	ratio = m_width / (float)m_height;
	glViewport(0, 0, m_width, m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width / 10, width / 10, -height / 10, height / 10, -350, 350);
	perspectiveGL(1, (GLfloat)m_width / (GLfloat)m_height, 0.1f, 350.0f);
	glMatrixMode(GL_MODELVIEW);
}

void MainMenuGUI::InitGL()
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

void MainMenuGUI::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void MainMenuGUI::close()
{
}
