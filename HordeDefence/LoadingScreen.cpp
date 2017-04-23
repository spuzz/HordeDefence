#include "LoadingScreen.h"



LoadingScreen::LoadingScreen()
{
}


LoadingScreen::~LoadingScreen()
{
}


void LoadingScreen::start()
{
	setupGL();
}

void LoadingScreen::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	draw();
	glLoadIdentity();

	glfwSwapBuffers(m_window.get());
	glfwPollEvents();
}

void LoadingScreen::close()
{

}

void LoadingScreen::loadGUI()
{
	init("../HordeDefenceArt/GUI");
	loadScheme("OgreTray.scheme");
	setFont("DejaVuSans-10");

	CEGUI::FrameWindow* background = (CEGUI::FrameWindow*)createWidget("OgreTray/StaticImage",
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f), "background");
	background->setProperty(background->RiseOnClickEnabledPropertyName, "false");
}

bool LoadingScreen::mouseAction(int button, int action)
{
	return true;
}

bool LoadingScreen::keyAction(int key, int scancode, int action)
{
	return true;
}

void LoadingScreen::setupGL()
{
	float ratio;
	glfwGetFramebufferSize(m_window.get(), &m_width, &m_height);

	perspectiveSetup();
	ratio = m_width / (float)m_height;
	InitGL();
}

void LoadingScreen::perspectiveSetup() {
	float ratio;
	ratio = m_width / (float)m_height;
	glViewport(0, 0, m_width, m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width / 10, width / 10, -height / 10, height / 10, -350, 350);
	perspectiveGL(1, (GLfloat)m_width / (GLfloat)m_height, 0.1f, 350.0f);
	glMatrixMode(GL_MODELVIEW);
}

void LoadingScreen::InitGL()
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

void LoadingScreen::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}