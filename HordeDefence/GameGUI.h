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
	void start();
	void update();
	void close();


	bool toggleGameMenu(int n_bActivate);
	bool onExitToDesktop(const CEGUI::EventArgs& e);
	bool quitCancel(const CEGUI::EventArgs& e);
	bool onReturnToGame(const CEGUI::EventArgs& e);
	bool checkScreenMoveKey(int key, int action);
	void TopologicalGraphSort(std::vector<std::shared_ptr<Sprite>>& nSprites);

	// Getters and Setters
	void setZoomFactor(float inZoom) { if (inZoom >= 6 && inZoom <= 12) { m_zoomFactor = inZoom; } }
	void increaseZoomFactor(float inZoomIncrease);
	void decreaseZoomFactor(float inZoomDecrease);
	void changeXScreenLoc(float inXScreenLoc);
	void changeYScreenLoc(float inYScreenLoc);
	void setYMovement(float inYMovement) { m_yIncreasing = inYMovement; }
	void setXMovement(float inXMovement) { m_xIncreasing = inXMovement; }
	float getXMovement() { return m_xIncreasing; }
	float getYMovement() { return m_yIncreasing; }

private:
	void loadGameGUI();
	void perspectiveSetup();
	void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void InitGL();
	void setupGL();
	bool m_bMenuUp;
	bool m_bBlockInput;

	int m_width,m_height;
	float m_viewZoomFactor;
	float m_zoomFactor;
	float m_yScreenLoc;
	float m_xScreenLoc;
	float m_zScreenLoc;
	float m_xIncreasing, m_yIncreasing;

};

