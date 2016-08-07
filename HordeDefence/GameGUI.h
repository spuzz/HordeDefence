#pragma once
#include "GUI.h"
class GameGUI :
	public GUI
{
public:
	GameGUI();
	virtual ~GameGUI();

	// base class pure virtual function definitions
	void loadGUI();
	bool mouseAction(int button, int action);
	bool keyAction(int key, int scancode, int action);



	bool toggleGameMenu(int n_bActivate);
	bool onExitToDesktop(const CEGUI::EventArgs& e);
	bool quitCancel(const CEGUI::EventArgs& e);
	// Getters and Setters
private:
	void loadGameGUI();

	bool m_bMenuUp;
	bool m_bBlockInput;
};

