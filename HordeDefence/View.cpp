

#include "View.h"
#include "Unit.h"


using std::shared_ptr;
using std::make_shared;




View::View()
{
	
	scale = 32;
	zoomFactor = 9;
	xScreenLoc = 0;
	yScreenLoc = -40;
	zScreenLoc = -220;
	viewZoomFactor = 55.0f;
	yIncreasing = 0;
	xIncreasing = 0;
	animTest = 0;
	animTimer = 0;
}

void View::setupGL(GLFWwindow* window)
{
	float ratio;
	glfwGetFramebufferSize(window, &width, &height);
	
	perspectiveSetup();
	ratio = width / (float)height;
	InitGL();
}

void View::perspectiveSetup() {
	float ratio;
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width / 10, width / 10, -height / 10, height / 10, -350, 350);
	perspectiveGL(viewZoomFactor/zoomFactor, (GLfloat)width / (GLfloat)height, 0.1f, 350.0f);
	glMatrixMode(GL_MODELVIEW);
}
void View::InitGL()
{
	
	if (!txtrLoader.LoadGLTextures())								// Jump To Texture Loading Routine
	{
		
		exit(0);									// If Texture Didn't Load Return FALSE
	}
	texture.push_back(txtrLoader.retrieveMapTexture()[0]);
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable(GL_LIGHT0);								// Quick And Dirty Lighting (Assumes Light0 Is Set Up)
	glEnable(GL_LIGHTING);								// Enable Lighting
	glEnable(GL_COLOR_MATERIAL);						// Enable Material Coloring
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

}

void View::draw(GLFWwindow* window, Model &model) 
{
	changeXScreenLoc(xIncreasing);
	changeYScreenLoc(yIncreasing);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	std::shared_ptr<Tmx::Map> map = model.getTmxMap();
	float width = map->GetWidth();
	float height = map->GetHeight();
	const Tmx::Tileset *set = map->GetTileset(0);
	int widthInTiles = map->GetTileset(0)->GetImage()->GetWidth()/map->GetTileset(0)->GetTileWidth();
	int heightInTiles = map->GetTileset(0)->GetImage()->GetHeight()/map->GetTileset(0)->GetTileHeight();
	for (auto tileLayer : map->GetTileLayers()) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {

				int test = tileLayer->GetTileTilesetIndex(y, x);
				int gid = tileLayer->GetTileId(y, x);
				const Tmx::MapTile tileTest = tileLayer->GetTile(y, x);

				if (tileTest.tilesetId == -1) {
					continue;
				}
				float tileset_col = (gid % widthInTiles);
				float tileset_row = gid / widthInTiles;
				glLoadIdentity();
				glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
				glTranslatef(((width - x - 1) - (height - y - 1)), ((width - x - 1) + (height - y - 1)) *0.5,0 );
				glBegin(GL_QUADS);
				glNormal3f(0.0f, 0.0f, 1.0f);
				
				glTexCoord2f((0.0f / widthInTiles) + tileset_col / widthInTiles, (1.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(-1.0f, -0.5f, 1.0f);
				glTexCoord2f((1.0f / widthInTiles) + tileset_col / widthInTiles, (1.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(1.0f, -0.5f, 1.0f);
				glTexCoord2f((1.0f / widthInTiles) + tileset_col / widthInTiles, (0.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(1.0f, 0.5f, 1.0f);
				glTexCoord2f((0.0f / widthInTiles) + tileset_col / widthInTiles, (0.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(-1.0f, 0.5f, 1.0f);
				glEnd();
			}

		}
	}
	std::vector<std::shared_ptr<Unit>> unitTest = model.getPlayerUnits();
	for (auto unit : unitTest) {
		std::vector<GLuint> character = 
			txtrLoader.retrieveUnitTexture(unit->getRace(), unit->getGender(), unit->getClassType(),
			unit->getEquippedWeapon().getType(), unit->getEquippedArmor().getType(), unit->getEquippedOffhand().getType());
		for (auto txtr : character) {

			glLoadIdentity();
			glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
			float x = unit->getLocation().first + 1 ;
			float y = unit->getLocation().second + 1;
			glTranslatef(x- y, (x + y) * 0.5, 0);
			int dir = unit->getDirection();
			dir -= 1;
			if (dir < 0) {dir = 7;}
			glBindTexture(GL_TEXTURE_2D, txtr);
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f((0.0f) + (floor(unit->getAnimation()->getCurrentAnimFrame()) / 32), (1.0f) / 8 + (dir / 8.0f)); glVertex3f(-2.0f, -2.0f, 1.0f);
			glTexCoord2f((1.0f / 32) + (floor(unit->getAnimation()->getCurrentAnimFrame()) / 32), (1.0f / 8) + (dir / 8.0f)); glVertex3f(2.0f, -2.0f, 1.0f);
			glTexCoord2f((1.0f / 32) + (floor(unit->getAnimation()->getCurrentAnimFrame()) / 32), 0.0f + (dir / 8.0f)); glVertex3f(2.0f, 2.0f, 1.0f);
			glTexCoord2f((0.0f / 32) + (floor(unit->getAnimation()->getCurrentAnimFrame()) / 32), (0.0f / 8) + (dir / 8.0f));  glVertex3f(-2.0f, 2.0f, 1.0f);
			glEnd();

		}
	}
	glLoadIdentity();
	glfwSwapBuffers(window);
	glfwPollEvents();
}


void View::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}


void View::increaseZoomFactor(float inZoomIncrease) 
{ 
	if ((zoomFactor + inZoomIncrease) >= 6 && (zoomFactor + inZoomIncrease) <= 20) {
		zoomFactor += inZoomIncrease;
	}
	perspectiveSetup();

}
void View::decreaseZoomFactor(float inZoomDecrease) 
{ 
	if ((zoomFactor + inZoomDecrease) >= 6 && (zoomFactor + inZoomDecrease) <= 20) {
		zoomFactor += inZoomDecrease;
	}
	perspectiveSetup();
}

void View::changeXScreenLoc(float inXScreenLoc)
{
	if ((xScreenLoc + inXScreenLoc) > -50 && (xScreenLoc + inXScreenLoc) < 50)
	{
		xScreenLoc += inXScreenLoc;
	}

}
void View::changeYScreenLoc(float inYScreenLoc)
{
	if ((yScreenLoc + inYScreenLoc) < -20 && (yScreenLoc + inYScreenLoc) > -80)
	{
		yScreenLoc += inYScreenLoc;
	}
}

View::~View()
{
}

