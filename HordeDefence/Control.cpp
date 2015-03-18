#include "Control.h"
#include "Input.h"



Control::Control(shared_ptr<View> inView, shared_ptr<Model> inModel) : mView(inView), mModel(inModel)
{
	glfwSetErrorCallback(&Input::error_callback);
	
	if (!glfwInit())
		exit(EXIT_FAILURE);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Horde Defence", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	Input &hexmap = Input::getInstance(); // initialize the singleton
	hexmap.setView(mView);
	hexmap.setModel(mModel);

	//The glfw callback is set up as follows:   
	//glfwSetMouseButtonCallback(&Input::mouseButtonCallback); // specifying the static callbac
	glfwSetKeyCallback(window, &Input::key_callback);
	glfwSetScrollCallback(window, &Input::mouseScroll_callback);
	glfwSetMouseButtonCallback(window, &Input::mouseClick_callback);
	mView->setupGL(window);
	while (!glfwWindowShouldClose(window))
	{
		mModel->update();
		mView->draw(window,*mModel);

	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

Control::~Control()
{

}