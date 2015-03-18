#include "textureLoader.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#define GL_CLAMP_TO_EDGE 0x812F
textureLoader::textureLoader()
{
}

AUX_RGBImageRec *textureLoader::LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File = NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File = fopen(Filename, "r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int textureLoader::LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	loadMaps();
	loadUnits();


	return 1;											// Return The Status
}

void textureLoader::loadUnits() 
{
	std::vector<std::string> list;
	int count = GetFileList("c:\\Users\\aspurrel\\Documents\\Art\\HordeDefenceArt\\Units\\Male\\Human\\*.png", list);
	string fileLocation;
	for (int i = 0; i < list.size(); i++) {
		fileLocation = "C:\\Users\\aspurrel\\Documents\\Art\\HordeDefenceArt\\Units\\Male\\Human\\" + list[i];
		string delim = ".";
		string token = list[i].substr(0,list[i].find(delim));
		createTexture(unitTextures,token , fileLocation);
	}
}

void textureLoader::loadMaps()
{

	std::vector<std::string> list;
	int count = GetFileList("c:\\Users\\aspurrel\\Documents\\Art\\HordeDefenceArt\\Maps\\*.png", list);
	string mapFileLocation;
	for (int i = 0; i < list.size(); i++) {
		mapFileLocation = "c:\\Users\\aspurrel\\Documents\\Art\\HordeDefenceArt\\Maps\\" + list[i];
		createTexture(mapTexture, i,mapFileLocation);
	}
	
}

void textureLoader::createTexture(std::vector<GLuint> &texture, const int &vectorLoc, const string &fileLocation)
{
	SDL_Surface *surfaceMap = IMG_Load(fileLocation.c_str());
	surfaceMap->w;
	surfaceMap->h;
	surfaceMap->pixels; 
	int bpp = surfaceMap->format->BytesPerPixel;

	mapTexture.push_back(nextTextureID);
	glGenTextures(1,&(texture[vectorLoc]));					// Create The Texture

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[vectorLoc]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surfaceMap->w, surfaceMap->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, surfaceMap->pixels);
	nextTextureID++;
}

void textureLoader::createTexture(std::map<string, GLuint> &texture, const string &mapKey, const string &fileLocation)
{
	SDL_Surface *surfaceMap = IMG_Load(fileLocation.c_str());
	surfaceMap->w;
	surfaceMap->h;
	surfaceMap->pixels;
	int bpp = surfaceMap->format->BytesPerPixel;

	mapTexture.push_back(nextTextureID);
	glGenTextures(1, &(texture[mapKey]));					// Create The Texture

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[mapKey]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surfaceMap->w, surfaceMap->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, surfaceMap->pixels);
	nextTextureID++;
}

int textureLoader::GetFileList(const char *searchkey, std::vector<std::string> &list) {


	WIN32_FIND_DATA fd;
	HANDLE h = FindFirstFile(searchkey, &fd);

	if (h == INVALID_HANDLE_VALUE)
	{
		return 0; // no files found
	}

	while (1)
	{
		list.push_back(fd.cFileName);

		if (FindNextFile(h, &fd) == FALSE)
			break;
	}
	return list.size();
}
std::vector<GLuint> textureLoader::retrieveUnitTexture(string race, bool gender, string unitClass, string weapon, string armor,string offhand)
{
	std::vector<GLuint> retreivedTxtrs;
	retreivedTxtrs.push_back(unitTextures[offhand]);
	retreivedTxtrs.push_back(unitTextures["male_head1"]);
	if (armor == "steel") { armor = "steel_armor"; }
	retreivedTxtrs.push_back(unitTextures[armor]);
	if (weapon == "sword") { weapon = "shortsword"; }
	retreivedTxtrs.push_back(unitTextures[weapon]);
	return retreivedTxtrs;
}

textureLoader::~textureLoader()
{
}
