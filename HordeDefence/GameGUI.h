#pragma once
#include "GUI.h"
#include "EasyBMP.h"
#include "UnitInfoButton.h"
#include "UnitInfoPanel.h"
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
	bool updateSelection(const bool& selectionChanged);
	bool onSelectUnit(const CEGUI::EventArgs& e);
	bool updateMinimap();
	bool loadMinimap();
	bool loadCommands();

private:
	void loadGameGUI();
	void perspectiveSetup();
	void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void InitGL();
	void setupGL();

	void createSelectedUnitButtons();
	void loadTextures();

	bool m_bMenuUp;
	bool m_bBlockInput;
	CEGUI::Texture* guiTex;
	CEGUI::Texture* unitText;
	CEGUI::Texture* unitHealthText;
	CEGUI::Texture* unitHealthBackgroundText;
	CEGUI::BasicImage* image;
	CEGUI::BasicImage* unitImage;
	CEGUI::FrameWindow* miniMap;
	std::vector<int> mUnitIDs;
	std::vector<UnitInfoButton> mUnitButtons;
	shared_ptr<UnitInfoPanel> mUnitInfoPanel;


	BMP AnImage;
	unsigned int *mPixelBuffer;
	

	BMP* mMiniMapImage;
};

