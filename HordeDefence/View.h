#pragma once
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include "Model.h"
#include "textureLoader.h"

#include <vector>
using std::shared_ptr;
class View
{
public:
	View();
	virtual ~View();
	void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void perspectiveSetup();
	void setupGL(GLFWwindow* window);
	virtual void draw(GLFWwindow* window, Model &model);

	void setZoomFactor(float inZoom) { if (inZoom >= 6 && inZoom <= 12) { zoomFactor = inZoom; } }
	void increaseZoomFactor(float inZoomIncrease);
	void decreaseZoomFactor(float inZoomDecrease);
	const float getZoomFactor() const { return zoomFactor; }
	const float getViewZoomFactor() const { return viewZoomFactor; }
	
	float getXScreenLoc() const{ return xScreenLoc; }
	float getYScreenLoc() const{ return yScreenLoc; }
	float getZScreenLoc() const{ return zScreenLoc; }
	void changeXScreenLoc(float inXScreenLoc);
	void changeYScreenLoc(float inYScreenLoc);
	void setYMovement(float inYMovement) { yIncreasing = inYMovement; }
	void setXMovement(float inXMovement) { xIncreasing = inXMovement; }
	float getXMovement() { return xIncreasing;  }
	float getYMovement() { return yIncreasing; }
protected:

private:
	void InitGL();
	float viewZoomFactor;
	float zoomFactor;
	float yScreenLoc;
	float xScreenLoc;
	float zScreenLoc;
	float xIncreasing, yIncreasing;
	float animTest, animTimer;
	GLuint	top,box;				// Storage For The Top Display List
	int scale;
	int width, height;
	std::vector<GLuint>	texture;			// Storage For 1 Texture
	textureLoader txtrLoader;
	GLenum texture_format;
	GLint  nOfColors;
	int test;
};