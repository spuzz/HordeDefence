#pragma once

#include "View.h"
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Control.h"
class Input
{
public:
	static Input& getInstance() // Singleton is accessed via getInstance()
	{
		static Input instance; // lazy singleton, instantiated on first use

		return instance;
	}

	static void error_callback(int error, const char* description)
	{
		fputs(description, stderr);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		getInstance().key_callbackImpl(window, key, scancode, action, mods);


	}
	void key_callbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

		if (mView->keyAction(key, scancode, action))
		{
			return;
		}
		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_T) {
			if (action == GLFW_PRESS) {
				mModel->actionOnLocation(0, 0);
			}
		}
	}

	static void mouseScroll_callback(GLFWwindow* window, double up, double down)
	{
		getInstance().mouseScroll_callbackImpl(window, up, down);
	}

	void mouseScroll_callbackImpl(GLFWwindow* window, double up, double down)
	{
		//mView->increaseZoomFactor(down);
	}

	static void mouseClick_callback(GLFWwindow * window, int button, int action, int mods)
	{
		getInstance().mouseClick_callbackImpl(window, button,action, mods);
	}


	void mouseClick_callbackImpl(GLFWwindow * window, int button, int action, int mods)
	{
		mView->mouseAction(button, action, mods);
		if (action == GLFW_PRESS) {
			
			double x_pos, y_pos;
			int width, height;
			GLfloat winZ;
			GLint viewport[4];                  // Where The Viewport Values Will Be Stored
			glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves The Viewport Values (X, Y, Width, Height)
			glfwGetFramebufferSize(window, &width, &height);
			GLdouble modelview[16];                 // Where The 16 Doubles Of The Modelview Matrix Are To Be Stored
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);       // Retrieve The Modelview Matrix
			GLdouble projection[16];                // Where The 16 Doubles Of The Projection Matrix Are To Be Stored
			glGetDoublev(GL_PROJECTION_MATRIX, projection);     // Retrieve The Projection Matrix
			glfwGetCursorPos(window, &x_pos, &y_pos);
			glReadPixels(width / 2, height / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
			glm::mat4 test = glm::mat4(projection[0], projection[1], projection[2], projection[3], projection[4], projection[5], projection[6],
				projection[7], projection[8], projection[9], projection[10], projection[11], projection[12], projection[13], projection[14], projection[15]);
			glm::mat4 E = glm::translate(glm::mat4(1.f), glm::vec3(mView->getXScreenLoc(), mView->getYScreenLoc(), mView->getZScreenLoc()));
			glm::mat4 F = glm::perspective(mView->getViewZoomFactor() / mView->getZoomFactor(), (GLfloat)width / (GLfloat)height, 0.1f, 350.0f);
			//glm::mat4 G = glm::inverse(F * E);
			glm::vec3 H = glm::unProject(glm::vec3(x_pos, height - y_pos, winZ), E, test, glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));


			pair<float, float> mouse2DPos = isoTo2D(pair<float, float>(H.x, H.y));

			if (button == GLFW_MOUSE_BUTTON_1)
			{
				mModel->selectOnLocation(mouse2DPos.first, mouse2DPos.second);
			}
			else if (button == GLFW_MOUSE_BUTTON_2)
			{

				mModel->actionOnLocation(mouse2DPos.first, mouse2DPos.second);
			}

		}

	}

	static void mouseButtonCallback(int key, int action) // this method is specified as glfw callback
	{
		//here we access the instance via the singleton pattern and forward the callback to the instance method
		getInstance().mouseButtonCallbackImpl(key, action);
	}

	void mouseButtonCallbackImpl(int key, int action) //this is the actual implementation of the callback method
	{
		//the callback is handled in this instance method           
		//... [CODE here]
	}

	static void cursorPos_callback(GLFWwindow * window, double xPos, double yPos)
	{
		getInstance().cursorPosCallbackImpl(xPos, yPos);
	}

	void cursorPosCallbackImpl(double xPos, double yPos)
	{
		mView->setMouseCursor(xPos, yPos);
	}

	static void setView(shared_ptr<View> inView)
	{
		getInstance().mView = shared_ptr<View>(inView);
	}
	static void setModel(shared_ptr<Model> inModel)
	{
		getInstance().mModel = shared_ptr<Model>(inModel);
	}

	static void setControl(shared_ptr<Control> inControl)
	{
		getInstance().mControl = shared_ptr<Control>(inControl);
	}

	pair<float, float> isoTo2D(pair<float, float> pt)
	{

		pair<float, float> tempPt;
		tempPt.first = (2 * pt.second + pt.first) / 2 + 0.5;
		tempPt.second = (2 * pt.second - pt.first) / 2 + 0.5;
		return(tempPt);
	}

	~Input();
private:
	Input(void) // private constructor necessary to allow only 1 instance
	{
	}
	shared_ptr<Model> mModel;
	shared_ptr<View> mView;
	shared_ptr<Control> mControl;
	Input(Input const&); // prevent copies
	void operator=(Input const&); // prevent assignments
};