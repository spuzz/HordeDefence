#pragma once
#include "GUI.h"
#include "EasyBMP.h"

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

	void start();
	void update();
	void close();


	bool toggleGameMenu(int n_bActivate);
	bool onExitToDesktop(const CEGUI::EventArgs& e);
	bool quitCancel(const CEGUI::EventArgs& e);
	bool onReturnToGame(const CEGUI::EventArgs& e);
	bool checkScreenMoveKey(int key, int action);
	void TopologicalGraphSort(std::vector<std::shared_ptr<GameObject>>& nGameObjects);
	bool updateSelection();
	bool onSelectUnit(const CEGUI::EventArgs& e);
	bool updateMinimap();

private:
	void loadGameGUI();
	void perspectiveSetup();
	void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void InitGL();
	void setupGL();
	
	bool m_bMenuUp;
	bool m_bBlockInput;
	CEGUI::Texture* guiTex;
	CEGUI::BasicImage* image;
	CEGUI::FrameWindow* miniMap;
	std::vector<int> mUnitIDs;
	int mUnitButtons;
	BMP AnImage;


};

