#include "Control.h"
#include "Input.h"



Control::Control(shared_ptr<Model>& nModel, shared_ptr<View>& nView)
{
	mModel = shared_ptr<Model>(nModel);
	mView = shared_ptr<View>(nView);
	mView->setControl(shared_ptr<Control>(this));
	
	InitGlfwWindow();
	SetupInput();

	mView->setupGL(window);
	worldCreation newWorld(mModel, mView);
	while (!glfwWindowShouldClose(window))
	{
		if (m_bPause == false)
		{
			mModel->update();
		}

		mView->draw(window,*mModel);

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
	//window = glfwCreateWindow(mode->width, mode->height, "Horde Defence", glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(1980, 1280, "Horde Defence", NULL, NULL);
	glfwSetWindowPos(window, 0, 25);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
}

void Control::SetupInput()
{
	Input &hexmap = Input::getInstance(); // initialize the singleton
	hexmap.setView(mView);
	hexmap.setModel(mModel);
	hexmap.setControl(shared_ptr<Control>(this));

	//The glfw callback is set up as follows:   
	//glfwSetMouseButtonCallback(&Input::mouseButtonCallback); // specifying the static callbac
	glfwSetKeyCallback(window, &Input::key_callback);
	glfwSetScrollCallback(window, &Input::mouseScroll_callback);
	glfwSetMouseButtonCallback(window, &Input::mouseClick_callback);
	glfwSetCursorPosCallback(window, &Input::cursorPos_callback);
}

void Control::exitGame()
{

}

bool Control::exitToDesktop(const CEGUI::EventArgs& e)
{
	glfwSetWindowShouldClose(window, GL_TRUE);
	return true;
}

Control::~Control()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}