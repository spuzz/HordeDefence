#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include "Model.h"
#include "textureLoader.h"
#include "GUI.h"
#include <vector>

enum Views
{
	MAINMENU,
	GAMEUI,
	SETTINGS,
	LOADING
};


using std::shared_ptr;
class View
{
public:
	View();
	virtual ~View();
	void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void perspectiveSetup();
	void setupGL(GLFWwindow* window);
	virtual void draw(GLFWwindow* window, Model &model);

	virtual bool keyAction(int key, int scancode, int action);
	virtual bool checkScreenMoveKey(int key, int action);

	bool mouseAction(int button, int action, int mods);

	virtual bool loadMenu(const Views& n_view, shared_ptr<GLFWwindow> n_window, shared_ptr<Model> n_model, const bool& closeCurrent = true);
	bool goBack();
	//virtual bool toggleGameMenu(int n_bActivate);

	bool View::onExitToDesktop(const CEGUI::EventArgs& e);
	bool View::quitCancel(const CEGUI::EventArgs& e);

	// Getters and Setters
	void setZoomFactor(float inZoom) { if (inZoom >= 6 && inZoom <= 12) { zoomFactor = inZoom; } }
	void increaseZoomFactor(float inZoomIncrease);
	void decreaseZoomFactor(float inZoomDecrease);
	const float getZoomFactor() const { return zoomFactor; }
	const float getViewZoomFactor() const { return viewZoomFactor; }
	
	float getXScreenLoc() const{ return m_gui->getXScreenLoc(); }
	float getYScreenLoc() const { return  m_gui->getYScreenLoc(); }
	float getZScreenLoc() const { return  m_gui->getZScreenLoc(); }
	void changeXScreenLoc(float inXScreenLoc);
	void changeYScreenLoc(float inYScreenLoc);
	void setYMovement(float inYMovement) { yIncreasing = inYMovement; }
	void setXMovement(float inXMovement) { xIncreasing = inXMovement; }
	float getXMovement() { return xIncreasing;  }
	float getYMovement() { return yIncreasing; }

	void setMouseCursor(double xPos, double yPos);
	//bool comparator(const std::shared_ptr<GameObject> &a, const std::shared_ptr<GameObject> &b);
	shared_ptr<textureLoader> getTextureLoader(void) { return txtrLoader;  }

	void setControl(shared_ptr<Control> n_ctrl) { m_Control = shared_ptr<Control>(n_ctrl); }

protected:
	void TopologicalGraphSort(std::vector<std::shared_ptr<GameObject>>& nGameObjects);

private:

	shared_ptr<Control> m_Control;
	void InitGL();
	float viewZoomFactor;
	float zoomFactor;
	float yScreenLoc;
	float xScreenLoc;
	float zScreenLoc;
	float xIncreasing, yIncreasing;
	float animTest, animTimer;
	GLuint	top,box;				// Storage For The Top Display List
	int scale;
	int width, height;
	std::vector<GLuint>	texture;			// Storage For 1 Texture
	shared_ptr<textureLoader> txtrLoader;
	GLenum texture_format;
	GLint  nOfColors;
	int test;
	shared_ptr<GUI> m_gui;
	std::vector<shared_ptr<GUI>> m_guiList;
	bool m_bMenuUp;
	bool m_bBlockInput;
};