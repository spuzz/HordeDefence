#pragma once

#include <memory>
#include "Model.h"
#include "View.h"
#include "worldCreation.h"
#include "Input.h"
using std::shared_ptr;

class Input;
class Control
{
public:
	Control(shared_ptr<Model>& nModel, shared_ptr<View>& nView);
	~Control();

	// Getters and Setters
	shared_ptr<View> getView(void) const { return mView; }
	shared_ptr<Model> getModel(void) const { return mModel; }

	virtual GLFWwindow* getWindow() const { return mWindow.get(); }

	void setPause(const bool& nPause) { m_bPause = nPause; }
	// Methods
	void Control::InitGlfwWindow();
	void Control::SetupInput();

	void NewGame(const CEGUI::EventArgs& e);
	void Settings(const CEGUI::EventArgs& e);
	bool exitToDesktop(const CEGUI::EventArgs& e);
	void mainMenu(const CEGUI::EventArgs& e);
	void RemoveLastView(const CEGUI::EventArgs& e);
	void exitGame();
protected:

private:
	//static void error_callback(int error, const char* description);
	//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void mouseScroll_callback(GLFWwindow* window, double up, double down);

	shared_ptr<Model> mModel;
	shared_ptr<View> mView;

	shared_ptr<GLFWwindow> mWindow;
	shared_ptr<Input> mHexmap;
	bool m_bKeepLastScene;
	bool m_bPause;
	bool m_bNewScene;
	Views m_newSceneType;
};