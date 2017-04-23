#pragma once

#include "GUI.h"
class LoadingScreen :
	public GUI
{
public:
	LoadingScreen();
	virtual ~LoadingScreen();

	// base class pure virtual function definitions
	void loadGUI();
	bool mouseAction(int button, int action);
	bool keyAction(int key, int scancode, int action);
	void start();
	void update();
	void close();

private:
	void perspectiveSetup();
	void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void InitGL();
	void setupGL();

	int m_width, m_height;
};
