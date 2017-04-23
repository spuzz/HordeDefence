#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <SDL.h>
#include "SDL_image.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include "bmp.h"
#include <vector>
#include "TmxLoader\Tmx.h"


using std::string;
class textureLoader
{
public:
	textureLoader();
	int LoadGLTextures();
	std::vector<GLuint> retrieveMapTexture() { return  mapTexture; }
	std::vector<GLuint> retrieveUnitTexture(string race, bool gender, string unitClass, string weapon, string armor,string offhand) const;
	std::vector<GLuint> retrieveUnitTexture(string race, bool gender) const;
	virtual void setCurrentTexture(const GLuint& nTxtr) { mCurrentTexture = nTxtr; }
	virtual const GLuint& getCurrentTexture() { return mCurrentTexture;  }
	virtual ~textureLoader();

private:
	AUX_RGBImageRec *LoadBMP(char *Filename);
	void loadUnits();
	void loadMaps();
	void createTexture(std::vector<GLuint> &texture, const int &vectorLoc,const string &fileLocation);
	void createTexture(std::map<string, GLuint> &texture, const string &mapKey, const string &fileLocation);
	int GetFileList(const char *searchkey, std::vector<std::string> &list);
	GLuint nextTextureID;
	std::vector<GLuint>	mapTexture;			// Storage For 1 Texture
	std::map<string, GLuint> unitTextures;

	GLuint mCurrentTexture;
};

