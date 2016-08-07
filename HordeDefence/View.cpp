

#include "View.h"
#include "Unit.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include "Control.h"
#include "GameGUI.h"

using std::shared_ptr;
using std::make_shared;


bool comparator(const std::shared_ptr<Sprite> &a, const std::shared_ptr<Sprite> &b) {
	return (a->getIsoDepth() < b->getIsoDepth());
}


View::View()
{
	
	scale = 32;
	zoomFactor = 9;
	xScreenLoc = 0;
	yScreenLoc = -40;
	zScreenLoc = -220;
	viewZoomFactor = 55.0f;
	yIncreasing = 0;
	xIncreasing = 0;
	animTest = 0;
	animTimer = 0;
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
	if (!txtrLoader.LoadGLTextures())								// Jump To Texture Loading Routine
	{
		
		exit(0);									// If Texture Didn't Load Return FALSE
	}
	loadMenu(Views::GAMEUI);
	//CEGUI::OpenGL3Renderer& gui = CEGUI::OpenGL3Renderer::bootstrapSystem();
	texture.push_back(txtrLoader.retrieveMapTexture()[0]);
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
	
	changeXScreenLoc(xIncreasing);
	changeYScreenLoc(yIncreasing);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	GLuint currentTxtr = 0;
	std::vector<std::shared_ptr<Sprite>>::iterator& it = model.getSprites().begin();
	std::vector<std::shared_ptr<Sprite>>& sprites = model.getSprites();
	TopologicalGraphSort(sprites);
	std::sort(sprites.begin(), sprites.end(),comparator);
	int count = 0;
	for (it = sprites.begin(); it != sprites.end(); ++it)
	{
		if (count == 0) 
		{
			count = 1;
		}
		else {
			(*it)->draw(xScreenLoc, yScreenLoc, zScreenLoc, txtrLoader);
		}
	}
	
	glLoadIdentity();
	m_gui->draw();

	glfwSwapBuffers(window);
	glfwPollEvents();
	
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


bool View::loadMenu(const Views& n_view)
{
	m_gui = shared_ptr<GUI>(new GameGUI());
	m_gui->setControl(m_Control);
	m_gui->loadGUI();
	return true;
}


// return false if no action was taken
bool View::keyAction(int key, int scancode, int action)
{

	if (m_gui->keyAction(key, scancode, action))
	{
		return true;
	}

	if (checkScreenMoveKey(key, action))
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

void View::increaseZoomFactor(float inZoomIncrease)
{
	if ((zoomFactor + inZoomIncrease) >= 6 && (zoomFactor + inZoomIncrease) <= 20) {
		zoomFactor += inZoomIncrease;
	}
	perspectiveSetup();

}
void View::decreaseZoomFactor(float inZoomDecrease)
{
	if ((zoomFactor + inZoomDecrease) >= 6 && (zoomFactor + inZoomDecrease) <= 20) {
		zoomFactor += inZoomDecrease;
	}
	perspectiveSetup();
}

void View::changeXScreenLoc(float inXScreenLoc)
{
	if ((xScreenLoc + inXScreenLoc) > -50 && (xScreenLoc + inXScreenLoc) < 50)
	{
		xScreenLoc += inXScreenLoc;
	}

}
void View::changeYScreenLoc(float inYScreenLoc)
{
	if ((yScreenLoc + inYScreenLoc) < -20 && (yScreenLoc + inYScreenLoc) > -80)
	{
		yScreenLoc += inYScreenLoc;
	}
}

// Determine dependencies for the topological graph sort
void View::TopologicalGraphSort(std::vector<std::shared_ptr<Sprite>>& nSprites)
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

View::~View()
{
}

