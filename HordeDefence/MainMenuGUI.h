#pragma once
#include "GUI.h"
class MainMenuGUI :
	public GUI
{
public:
	MainMenuGUI();
	~MainMenuGUI();

	// base class pure virtual function definitions
	void loadGUI();
	bool mouseAction(int button, int action);
	bool keyAction(int key, int scancode, int action);
	void start();
	void update();
	void close();

	bool onExitToDesktop(const CEGUI::EventArgs& e);
	bool onNewGame(const CEGUI::EventArgs& e);

private:
	void perspectiveSetup();
	void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void InitGL();
	void setupGL();

	int m_width, m_height;
};

