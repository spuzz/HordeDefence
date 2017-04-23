#include "Control.h"
#include <chrono>


Control::Control(shared_ptr<Model>& nModel, shared_ptr<View>& nView)
{
	mModel = shared_ptr<Model>(nModel);
	mView = shared_ptr<View>(nView);
	mView->setControl(shared_ptr<Control>(this));

	InitGlfwWindow();
	SetupInput();

	//mView->setupGL(window);
	m_bNewScene = false;
	m_bKeepLastScene = false;
	mView->loadMenu(Views::GAMEUI, mWindow, mModel);

	auto time = std::chrono::high_resolution_clock::now();
	auto lastTime = time;
	float tickTime = std::chrono::duration_cast<std::chrono::seconds>(time - lastTime).count();
	while (!glfwWindowShouldClose(mWindow.get()))
	{

		if (m_bNewScene == true)
		{
			mView->loadMenu(m_newSceneType, mWindow, mModel,m_bKeepLastScene);
			auto lastTime = std::chrono::high_resolution_clock::now();

			m_bNewScene = false;
		}

		time = std::chrono::high_resolution_clock::now();
		tickTime = std::chrono::duration_cast<std::chrono::milliseconds>(time - lastTime).count() / 1000.0f;
		if (m_bPause == false)
		{
			mModel->update(tickTime);
		}
		lastTime = time;
		mView->draw(mWindow.get(), *mModel);

	}

}


void Control::InitGlfwWindow()
{
	glfwSetErrorCallback(&Input::error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	mWindow = shared_ptr<GLFWwindow>(glfwCreateWindow(mode->width, mode->height, "Horde Defence", glfwGetPrimaryMonitor(), NULL));
	//window = glfwCreateWindow(1980, 1280, "Horde Defence", NULL, NULL);
	glfwSetWindowPos(mWindow.get(), 0, 25);
	if (!mWindow.get())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(mWindow.get());
	glfwSwapInterval(1);
}

void Control::SetupInput()
{
	mHexmap = shared_ptr<Input>(&Input::getInstance()); // initialize the singleton
	mHexmap->setView(mView);
	mHexmap->setModel(mModel);
	mHexmap->setControl(shared_ptr<Control>(this));

	//The glfw callback is set up as follows:   
	//glfwSetMouseButtonCallback(&Input::mouseButtonCallback); // specifying the static callbac
	glfwSetKeyCallback(mWindow.get(), &Input::key_callback);
	glfwSetScrollCallback(mWindow.get(), &Input::mouseScroll_callback);
	glfwSetMouseButtonCallback(mWindow.get(), &Input::mouseClick_callback);
	glfwSetCursorPosCallback(mWindow.get(), &Input::cursorPos_callback);
}

void Control::exitGame()
{

}

bool Control::exitToDesktop(const CEGUI::EventArgs& e)
{
	glfwSetWindowShouldClose(mWindow.get(), GL_TRUE);
	return true;
}

void Control::mainMenu(const CEGUI::EventArgs& e)
{
	m_bKeepLastScene = false;
	m_bNewScene = true;
	m_newSceneType = MAINMENU;
}

void Control::NewGame(const CEGUI::EventArgs& e)
{
	m_bKeepLastScene = false;
	m_bNewScene = true;
	m_newSceneType = GAMEUI;
}

void Control::Settings(const CEGUI::EventArgs& e)
{
	m_bKeepLastScene = true;
	m_bNewScene = true;
	m_newSceneType = SETTINGS;
	
	
}

void Control::RemoveLastView(const CEGUI::EventArgs& e)
{

	mView->goBack();
}


Control::~Control()
{
	glfwDestroyWindow(mWindow.get());
	glfwTerminate();
	exit(EXIT_SUCCESS);
}