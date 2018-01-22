

#include "View.h"
#include "Unit.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include "Control.h"
#include "GameGUI.h"
#include "MainMenuGUI.h"
#include "LoadingScreen.h"
#include "SettingsGUI.h"

using std::shared_ptr;
using std::make_shared;


//bool comparator(const std::shared_ptr<GameObject> &a, const std::shared_ptr<GameObject> &b) {
//	return (a->getIsoDepth() < b->getIsoDepth());
//}


View::View()
{
	mInit = false;
	//scale = 32;
	//zoomFactor = 9;
	//xScreenLoc = 0;
	//yScreenLoc = -40;
	//zScreenLoc = -220;
	//viewZoomFactor = 55.0f;
	//yIncreasing = 0;
	//xIncreasing = 0;
	//animTest = 0;
	//animTimer = 0;
}

void View::setupGL(GLFWwindow* window)
{
	float ratio;
	glfwGetFramebufferSize(window, &width, &height);
	
	perspectiveSetup();
	ratio = width / (float)height;
	InitGL();
}

void View::perspectiveSetup() {
	float ratio;
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width / 10, width / 10, -height / 10, height / 10, -350, 350);
	perspectiveGL(viewZoomFactor/zoomFactor, (GLfloat)width / (GLfloat)height, 0.1f, 350.0f);
	glMatrixMode(GL_MODELVIEW);
}
void View::InitGL()
{
	//if (!txtrLoader->LoadGLTextures())								// Jump To Texture Loading Routine
	//{
	//	
	//	exit(0);									// If Texture Didn't Load Return FALSE
	//}
	////loadMenu(Views::GAMEUI);
	////CEGUI::OpenGL3Renderer& gui = CEGUI::OpenGL3Renderer::bootstrapSystem();
	//texture.push_back(txtrLoader->retrieveMapTexture()[0]);
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

void View::draw(GLFWwindow* window, Model &model) 
{
	//changeXScreenLoc(xIncreasing);
	//changeYScreenLoc(yIncreasing);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	//GLuint currentTxtr = 0;
	//std::vector<std::shared_ptr<GameObject>>::iterator& it = model.getGameObjects().begin();
	//std::vector<std::shared_ptr<GameObject>>& GameObjects = model.getGameObjects();
	//TopologicalGraphSort(GameObjects);
	//std::sort(GameObjects.begin(), GameObjects.end(),comparator);
	//int count = 0;
	//for (it = GameObjects.begin(); it != GameObjects.end(); ++it)
	//{
	//	if (count == 0) 
	//	{
	//		count = 1;
	//	}
	//	else {
	//		(*it)->draw(xScreenLoc, yScreenLoc, zScreenLoc, txtrLoader);
	//	}
	//}
	//
	//glLoadIdentity();
	m_gui->update();

	//glfwSwapBuffers(window);
	//glfwPollEvents();
	
}


void View::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}


bool View::loadMenu(const Views& n_view, shared_ptr<GLFWwindow> n_window, shared_ptr<Model> n_model, const bool& closeCurrent)
{

	if (closeCurrent && m_guiList.size() > 1)
	{
		
		m_guiList[(m_guiList.size() - 1)]->close();
		m_guiList[(m_guiList.size() - 1)].reset();
		m_guiList.pop_back();
	}
	if (mInit == false)
	{
		txtrLoader = shared_ptr<textureLoader>(new textureLoader());
		if (!txtrLoader->LoadGLTextures())								// Jump To Texture Loading Routine
		{

			exit(0);									// If Texture Didn't Load Return FALSE
		}
		mInit = true;
	}

	switch(n_view)
	{
	case GAMEUI:
		m_gui = shared_ptr<GUI>(new GameGUI());	
		break;
	case MAINMENU:
		m_gui = shared_ptr<GUI>(new MainMenuGUI());
		break;
	case LOADING:
		m_gui = shared_ptr<GUI>(new LoadingScreen());
		break;
	case SETTINGS:
		m_gui = shared_ptr<GUI>(new SettingsGUI());
		break;
	}
	
	m_gui->setTextureLoader(txtrLoader);
	m_gui->setControl(m_Control);
	m_gui->setWindow(n_window);
	m_gui->setModel(n_model);
	m_gui->loadGUI();
	m_gui->start();
	m_guiList.push_back(m_gui);
	return true;
}

bool View::goBack()
{
	if (m_guiList.size() > 1)
	{
		m_guiList[(m_guiList.size() - 1)]->close();
		m_guiList[(m_guiList.size() - 1)].reset();
		m_guiList.pop_back();
		m_gui = shared_ptr<GUI>(m_guiList[(m_guiList.size() - 1)]);
		return true;
	}

	return false;
}
// return false if no action was taken
bool View::keyAction(int key, int scancode, int action)
{

	if (m_gui->keyAction(key, scancode, action))
	{
		return true;
	}



	return false;
	
}

// return false if no action was taken
bool View::mouseAction(int button, int action, int mods)
{
	m_gui->mouseAction(button,action);
	return false;

}

bool View::checkScreenMoveKey(int key, int action)
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
	else if(action == GLFW_RELEASE)
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


void View::setMouseCursor(double xPos, double yPos)
{
	m_gui->setMousePosition(xPos,yPos);
}


View::~View()
{
}

