#pragma once

#include <memory>
#include "Model.h"
#include "View.h"

using std::shared_ptr;

class Control
{
public:
	Control(shared_ptr<View> inView, shared_ptr<Model> inModel);
	~Control();

	shared_ptr<View> getView(void) const { return mView; }
	shared_ptr<Model> getModel(void) const { return mModel; }

	virtual GLFWwindow* getWindow() const { return window; }
protected:

private:
	//static void error_callback(int error, const char* description);
	//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void mouseScroll_callback(GLFWwindow* window, double up, double down);

	shared_ptr<Model> mModel;
	shared_ptr<View> mView;

	GLFWwindow* window;
};