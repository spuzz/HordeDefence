#include "SettingsGUI.h"

#include "Control.h"

SettingsGUI::SettingsGUI()
{
}


SettingsGUI::~SettingsGUI()
{
}
void SettingsGUI::start()
{
	setupGL();
}



void SettingsGUI::close()
{

}

void SettingsGUI::loadGUI()
{
	init("../HordeDefenceArt/GUI");
	loadScheme("OgreTray.scheme");
	setFont("DejaVuSans-10");

	CEGUI::FrameWindow* background = (CEGUI::FrameWindow*)createWidget("OgreTray/StaticImage",
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f), "background");
	background->setProperty(background->RiseOnClickEnabledPropertyName, "false");
	CEGUI::FrameWindow* menuFrame = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/FrameWindow",
		glm::vec4(0.4f, 0.3f, 0.2f, 0.24f), glm::vec4(0.0f), "menuFrame"));
	menuFrame->setDisabled(true);
	CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.41f, 0.32f, 0.18f, 0.05f), glm::vec4(0.0f), "Back"));
	button->setText("Back");
	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Control::RemoveLastView, &(*m_Control)));
}

void SettingsGUI::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	draw();
	glLoadIdentity();

	glfwSwapBuffers(m_window.get());
	glfwPollEvents();
}

bool SettingsGUI::mouseAction(int button, int action)
{
	injectMouseAction(button, action);
	return true;
}

bool SettingsGUI::keyAction(int key, int scancode, int action)
{
	return true;
}

void SettingsGUI::setupGL()
{
	float ratio;
	glfwGetFramebufferSize(m_window.get(), &m_width, &m_height);

	perspectiveSetup();
	ratio = m_width / (float)m_height;
	InitGL();
}

void SettingsGUI::perspectiveSetup() {
	float ratio;
	ratio = m_width / (float)m_height;
	glViewport(0, 0, m_width, m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width / 10, width / 10, -height / 10, height / 10, -350, 350);
	perspectiveGL(1, (GLfloat)m_width / (GLfloat)m_height, 0.1f, 350.0f);
	glMatrixMode(GL_MODELVIEW);
}

void SettingsGUI::InitGL()
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

void SettingsGUI::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}