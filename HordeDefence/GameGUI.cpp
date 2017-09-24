#include "GameGUI.h"
#include "Control.h"
#include <sstream>

bool comparator(const std::shared_ptr<GameObject> &a, const std::shared_ptr<GameObject> &b) {
	return (a->getIsoDepth() < b->getIsoDepth());
}


GameGUI::GameGUI()
{
	m_zoomFactor = 9;
	m_xScreenLoc = 0;
	m_yScreenLoc = -40;
	m_zScreenLoc = -220;
	m_viewZoomFactor = 255.0f;
	m_yIncreasing = 0;
	m_xIncreasing = 0;

	// Minimap
	// Set size to 640 × 480
	AnImage.SetSize(640, 480);
	// Set its color depth to 32-bits
	AnImage.SetBitDepth(32);
}

void GameGUI::start()
{
	setupGL();
}

void GameGUI::update()
{
	changeXScreenLoc(m_xIncreasing);
	changeYScreenLoc(m_yIncreasing);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	GLuint currentTxtr = 0;
	std::vector<std::shared_ptr<GameObject>>::iterator& it = m_model->getGameObjects().begin();
	std::vector<std::shared_ptr<GameObject>>& GameObjects = m_model->getGameObjects();
	TopologicalGraphSort(GameObjects);
	std::sort(GameObjects.begin(), GameObjects.end(), comparator);
	int count = 0;
	for (it = GameObjects.begin(); it != GameObjects.end(); ++it)
	{
		if (count == 0)
		{
			count = 1;
		}
		else {
			(*it)->draw(m_xScreenLoc, m_yScreenLoc, m_zScreenLoc, *m_txtrLoader.get());
		}
	}
	draw();
	glLoadIdentity();

	glfwSwapBuffers(m_window.get());
	glfwPollEvents();

	if (m_model->getSelectionChanged() == true)
	{
		updateSelection(true);
		m_model->setSelectionChanged(false);
	}
	else
	{
		updateSelection(false);
	}
	//updateMinimap();
}

void GameGUI::close()
{

}

void GameGUI::loadGUI()
{
	init("../HordeDefenceArt/GUI");
	loadScheme("OgreTray.scheme");
	setFont("DejaVuSans-10");
	loadGameGUI();
	//loadMinimap();
	loadCommands();
	showMouseCursor();
}

void GameGUI::loadGameGUI()
{
	m_bBlockInput = false;

	// Mini map pane
	CEGUI::FrameWindow* mapFrame = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/FrameWindow",
		glm::vec4(0.0f, 0.70f, 0.2f, 0.30f), glm::vec4(0.0f), "MapFrame"));

	loadScheme("TaharezLook.scheme");
	miniMap = static_cast<CEGUI::FrameWindow*>(createWidget("TaharezLook/ImageButton",
		glm::vec4(0.01f, 0.73f, 0.18f, 0.22f), glm::vec4(0.0f), "MiniMap"));
	loadScheme("OgreTray.scheme");
	//Unit info pane
	CEGUI::FrameWindow* bottomFrame = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/FrameWindow",
		glm::vec4(0.2f, 0.80f, 0.6f, 0.20f), glm::vec4(0.0f), "bottomFrame"));

	CEGUI::FrameWindow* unitList = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/AltStaticImage",
		glm::vec4(0.21f, 0.82f, 0.38f, 0.18f), glm::vec4(0.0f), "UnitList"));

	CEGUI::FrameWindow* unitInfo = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/AltStaticImage",
		glm::vec4(0.61f, 0.82f, 0.18f, 0.18f), glm::vec4(0.0f), "UnitInfo"));


	//Unit actions pane
	CEGUI::FrameWindow* unitActionFrame = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/FrameWindow",
		glm::vec4(0.8f, 0.70f, 0.2f, 0.30f), glm::vec4(0.0f), "unitActionFrame"));

	CEGUI::FrameWindow* groupActions = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/AltStaticImage",
		glm::vec4(0.81f, 0.71f, 0.18f, 0.28f), glm::vec4(0.0f), "GroupActions"));

	guiTex = createTexture("Texture");
	image = (CEGUI::BasicImage*)(&CEGUI::ImageManager::getSingleton().create("BasicImage", "RTTImage"));

	loadTextures();
	createSelectedUnitButtons();
	
	mUnitInfoPanel = shared_ptr<UnitInfoPanel>(new UnitInfoPanel(this,0.35,0.82));
	mUnitInfoPanel->setVisible(false);
	m_bMenuUp = false;

}

bool GameGUI::loadCommands()
{
	CEGUI::PushButton* command1 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.82f, 0.725f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command1"));
	command1->setText("Move");
	CEGUI::PushButton* command2 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.8733f, 0.725f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command2"));
	command2->setText("Stop");
	CEGUI::PushButton* command3 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.9266f, 0.725f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command3"));
	command3->setText("Attack");
	CEGUI::PushButton* command4 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.82f, 0.81f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command4"));
	command4->setText("Attack Move");
	CEGUI::PushButton* command5 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.8733f, 0.81f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command5"));
	command5->setText("Guard");
	CEGUI::PushButton* command6 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.9266f, 0.81f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command6"));
	command6->setText("Retreat");
	CEGUI::PushButton* command7 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.82f, 0.895f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command7"));
	command7->setText("Ability 1");
	CEGUI::PushButton* command8 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.8733f, 0.895f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command8"));
	command8->setText("Ability 2");
	CEGUI::PushButton* command9 = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
		glm::vec4(0.9266f, 0.895f, 0.0533f, 0.085f), glm::vec4(0.0f), "Command9"));
	command9->setText("Ability 3");
	command9->setVisible(false);
	//command->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameGUI::onSelectUnit, this));
	//unitImage->setTexture(unitText);
	//command->setProperty("NormalImage", "unitImage");
	//command->setProperty("HoverImage", "unitImage");
	//command->setProperty("PushedImage", "unitImage");
	//command->setProperty("DisabledImage", "unitImage");
	return true;
}

void GameGUI::loadTextures()
{


	for (auto type : m_model->getUnitTypes())
	{
		for (auto unitClass : m_model->getUnitTypeClasses(type))
		{
			string textName;
			if (unitClass == "")
			{
				textName = string(type);
			}
			else
			{
				textName = string(type + "_" + unitClass);
			}
			string texPath = "..\\HordeDefenceArt\\Units\\portraits\\" + textName + ".png";
			CEGUI::Texture* text = createTexture(textName);
			text->loadFromFile(texPath, "Units");
			mTextures[textName] = text;
		}
	}
	CEGUI::Texture* text = createTexture("EmptyBar");
	text->loadFromFile("..\\HordeDefenceArt\\HealthBar\\EmptyBar.png", "UI");
	mTextures["EmptyBar"] = text;

	text = createTexture("GreenBar");
	text->loadFromFile("..\\HordeDefenceArt\\HealthBar\\GreenBar.png", "UI");
	mTextures["GreenBar"] = text;

	text = createTexture("Icons");
	text->loadFromFile("..\\HordeDefenceArt\\Icons\\fantasy-tileset.png", "UI");
	mTextures["Icons"] = text;
}

void GameGUI::createSelectedUnitButtons()
{
	// 1 button for single unit selection - twice size
	shared_ptr<ImageButton> btn = createImageButton(glm::vec4(0.22f, 0.835f, 0.10f, 0.12f), "unit" + std::to_string(0), "Default");
	shared_ptr<ImageButton> btn2 = createImageButton(glm::vec4(0.22f, 0.955f, 0.10f, 0.02f), "emptyhealthbar" + std::to_string(0), "EmptyBar");
	shared_ptr<ImageButton> btn3 = createImageButton(glm::vec4(0.22f, 0.955f, 0.10f, 0.02f), "healthbar" + std::to_string(0), "GreenBar");

	UnitInfoButton infoBtn(btn, btn2, btn3);
	infoBtn.setEnabled(false);
	mUnitButtons.push_back(infoBtn);



	for (int a = 0; a < 6; a++)
	{
		shared_ptr<ImageButton> btn = createImageButton(glm::vec4(0.22f + (a * 0.05f), 0.835f, 0.05f, 0.06f), "unit" + std::to_string(a + 1), "Default");

		shared_ptr<ImageButton> btn2 = createImageButton(glm::vec4(0.22f + (a * 0.05f), 0.895f, 0.05f, 0.01f), "emptyhealthbar" + std::to_string(a + 1), "EmptyBar");
		shared_ptr<ImageButton> btn3 = createImageButton(glm::vec4(0.22f + (a * 0.05f), 0.895f, 0.05f, 0.01f), "healthbar" + std::to_string(a + 1), "GreenBar");

		UnitInfoButton infoBtn(btn, btn2, btn3);
		infoBtn.setEnabled(false);
		mUnitButtons.push_back(infoBtn);
	}


}


bool GameGUI::updateSelection(const bool& selectionChanged)
{
	if (selectionChanged == true)
	{

		
		// Activite large info button and add unit info to bar if only 1 unit selected
		string textName;
		if (m_model->getSelectedUnits().size() == 1)
		{
			// renabling only way to allow retexture?
			mUnitButtons[0].setEnabled(false);
			mUnitButtons[0].setEnabled(true);
			
			mUnitInfoPanel->setVisible(true);
			
			if (m_model->getSelectedUnits().begin()->second->getClassType() == "")
			{
				textName = string(m_model->getSelectedUnits().begin()->second->getRace());
			}
			else
			{
				textName = string(m_model->getSelectedUnits().begin()->second->getRace() + "_" + m_model->getSelectedUnits().begin()->second->getClassType());
			}


			mUnitButtons[0].mUnitImage->changeButtonImage(mTextures[textName]);
			for (int a = 1; a < 7; a++)
			{
				mUnitButtons[a].setEnabled(false);
			}
		}
		else
		{
			int count = 1;
			mUnitButtons[0].setEnabled(false);
			mUnitInfoPanel->setVisible(false);
			for (auto unit : m_model->getSelectedUnits())
			{
				// renabling only way to allow retexture?
				mUnitButtons[count].setEnabled(false);
				mUnitButtons[count].setEnabled(true);

				string textName;
				if (unit.second->getClassType() == "")
				{
					textName = string(unit.second->getRace());
				}
				else
				{
					textName = string(unit.second->getRace() + "_" + unit.second->getClassType());
				}

				mUnitButtons[count].mUnitImage->changeButtonImage(mTextures[textName]);
				count++;

			}
			for (int a = count; a < 7; a++)
			{
				mUnitButtons[a].setEnabled(false);
			}
		}

	}

	
	if (m_model->getSelectedUnits().size() == 1)
	{
		float perc = m_model->getSelectedUnits().begin()->second->getCurrentHealth() / m_model->getSelectedUnits().begin()->second->getMaxHealth();
		mUnitButtons[0].mHealthBar->changeButtonSize(glm::vec2(0.10f * perc, 0.02f), glm::vec2(0.0f));
		mUnitButtons[0].mHealthBar->changeTextureRatio(perc, 1);
		mUnitInfoPanel->updateInfo(m_model->getSelectedUnits().begin()->second);
	}
	else
	{
		int count = 0;
		for (auto unit : m_model->getSelectedUnits())
		{
			float perc = unit.second->getCurrentHealth() / unit.second->getMaxHealth();
			mUnitButtons[count].mHealthBar->changeButtonSize(glm::vec2(0.05f * perc, 0.01f), glm::vec2(0.0f));
			mUnitButtons[count].mHealthBar->changeTextureRatio(perc, 1);

			count++;
		}
	}


	
	//for (int a = 0; a < mUnitIDs.size(); a++)
	//{
	//	std::stringstream ss;
	//	ss << mUnitIDs[a];
	//	string str = ss.str();
	//	string unitButtonName = "UnitButton" + str;
	//	string unitHealthName = "UnitHealth" + str;
	//	string unitHealthBackground = "UnitHealthBackground" + str;
	//	destroyWidget(unitButtonName);
	//	destroyWidget(unitHealthName);
	//	destroyWidget(unitHealthBackground);
	//}

	//mUnitIDs.clear();
	//int count = 0;
	//for (auto unit : m_model->getSelectedUnits())
	//{
	//	mUnitIDs.push_back(unit.second->getObjectID());
	//	std::stringstream ss;
	//	ss << unit.second->getObjectID();
	//	string str = ss.str();
	//	string unitButtonName = "UnitButton" + str;
	//	string unitHealthName = "UnitHealth" + str;
	//	string unitHealthBackground = "UnitHealthBackground" + str;
	//	CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(createWidget("TaharezLook/ImageButton",
	//		glm::vec4(0.22f + (count*0.05f), 0.835f, 0.05f, 0.06f), glm::vec4(0.0f), unitButtonName));
	//	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameGUI::onSelectUnit, this));
	//	
	//	unitImage->setTexture(unitText);
	//	button->setProperty("NormalImage", "unitImage");
	//	button->setProperty("HoverImage", "unitImage");
	//	button->setProperty("PushedImage", "unitImage");
	//	button->setProperty("DisabledImage", "unitImage");
	//	button->setVisible(false);

	//	button = static_cast<CEGUI::PushButton*>(createWidget("TaharezLook/ImageButton",
	//		glm::vec4(0.22f + (count*0.05f), 0.895f, 0.05f, 0.01f), glm::vec4(0.0f), unitHealthBackground));
	//	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameGUI::onSelectUnit, this));
	//	button->setProperty("NormalImage", "unitImage");
	//	button->setProperty("HoverImage", "unitImage");
	//	button->setProperty("PushedImage", "unitImage");
	//	button->setProperty("DisabledImage", "unitImage");

	//	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameGUI::onSelectUnit, this));
	//	count++;
	//}
	return true;
}


bool GameGUI::loadMinimap()
{
	mMiniMapImage = new BMP();
	mMiniMapImage->ReadFromFile("..\\HordeDefenceArt\\Minimap\\minimap.bmp");
	mPixelBuffer = new unsigned int[mMiniMapImage->TellWidth() * mMiniMapImage->TellHeight()];
	memset(mPixelBuffer, 0, mMiniMapImage->TellWidth() * mMiniMapImage->TellHeight() * sizeof(unsigned int));

	for (int i = 0; i<mMiniMapImage->TellWidth(); i++)

	{

		for (int j = 0; j<mMiniMapImage->TellHeight(); j++)

		{
			mPixelBuffer[j * mMiniMapImage->TellWidth() + i] = (unsigned int)(((*mMiniMapImage)(i, j)->Red << 0) | ((*mMiniMapImage)(i, j)->Green << 8) | ((*mMiniMapImage)(i, j)->Blue << 16) | (0 << 24));

		}

	}

	int startLocX = 256;
	int startLocY = 384;

	std::map<std::pair<int, int>, shared_ptr<Tile>> tiles = m_model->getTiles();
	int width = m_model->getMapWidth();
	int height = m_model->getMapHeight();
	int widthPixels = 512 / width;
	int heightPixels = 512 / height;
	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
		{
			int r, g, b;
			if (tiles[pair<int, int>(x,y)]->isWalkable())
			{

				r = 0, g = 150, b = 0;
			}
			else
			{
				r = 0, g = 0, b = 150;

			}
			int xLoc = startLocX + ((x - y) * widthPixels / 2);
			int yLoc = startLocY - ((x + y) * heightPixels / 4);
			for (int i = 0; i<widthPixels; i++)
			{
				for (int n = -(i / 2); n <= i / 2; n++)
				{
					mPixelBuffer[(yLoc + n) * mMiniMapImage->TellWidth() + xLoc + i - widthPixels / 2] = (unsigned int)((r << 0) | (g << 8) | (b << 16) | (255 << 24));
				}

			}
			for (int i = widthPixels; i<widthPixels * 2; i++)
			{
				int negativeValue = i - widthPixels;
				for (int n = -((widthPixels - negativeValue) / 2); n <= (widthPixels - negativeValue) / 2; n++)
				{
					mPixelBuffer[(yLoc + n) * mMiniMapImage->TellWidth() + xLoc + i - widthPixels / 2] = (unsigned int)((r << 0) | (g << 8) | (b << 16) | (255 << 24));
				}
			}
		}
	}
	return true;
}

bool GameGUI::updateMinimap()
{
	int startLocX = 256;
	int startLocY = 384;
	int width = m_model->getMapWidth();
	int height = m_model->getMapHeight();
	int widthPixels = 512 / width;
	int heightPixels = 512 / height;
	unsigned int* pixelBuffer = new unsigned int[mMiniMapImage->TellWidth() * mMiniMapImage->TellHeight()];
	memcpy(pixelBuffer, mPixelBuffer, mMiniMapImage->TellWidth() * mMiniMapImage->TellHeight() * sizeof(unsigned int));
	std::map<int, std::shared_ptr<Unit>> units = m_model->getAllUnits();
	
	for (auto unit : units)
	{
		int x = unit.second->getIsoLocation().x;
		int y = unit.second->getIsoLocation().y;
		int r, g, b;
		if (unit.second->getPlayer() == 0)
		{

			r = 0, g = 255, b = 0;
		}
		else
		{
			r = 255, g = 0, b = 0;

		}
		int xLoc = startLocX + ((x - y) * widthPixels / 2);
		int yLoc = startLocY - ((x + y) * heightPixels / 4);
		for (int i = 0; i<widthPixels; i++)
		{
			for (int n = -(i / 2); n <= i / 2; n++)
			{
				pixelBuffer[(yLoc + n) * mMiniMapImage->TellWidth() + xLoc + i - widthPixels / 2] = (unsigned int)((r << 0) | (g << 8) | (b << 16) | (255 << 24));
			}

		}
		for (int i = widthPixels; i<widthPixels * 2; i++)
		{
			int negativeValue = i - widthPixels;
			for (int n = -((widthPixels - negativeValue) / 2); n <= (widthPixels - negativeValue) / 2; n++)
			{
				pixelBuffer[(yLoc + n) * mMiniMapImage->TellWidth() + xLoc + i - widthPixels / 2] = (unsigned int)((r << 0) | (g << 8) | (b << 16) | (255 << 24));
			}
		}
	}

	guiTex->loadFromMemory(pixelBuffer, CEGUI::Sizef(mMiniMapImage->TellWidth(), mMiniMapImage->TellHeight()), CEGUI::Texture::PF_RGBA);

	//// put the texture in an image
	delete pixelBuffer;
	const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), guiTex->getOriginalDataSize());
	image->setTexture(guiTex);
	image->setArea(rect);
	image->setAutoScaled(CEGUI::ASM_Both);

	// set the image on the static window
	miniMap->setProperty("NormalImage", "RTTImage");
	miniMap->setProperty("HoverImage", "RTTImage");
	miniMap->setProperty("PushedImage", "RTTImage");
	miniMap->setProperty("DisabledImage", "RTTImage");
	return true;
	//testButton->setProperty("HoverImage", "RTTImage");
}

/*  n_bActivate
0 = only close
1 = only open
2 = toggle
*/
bool GameGUI::toggleGameMenu(int n_bActivate)
{
	if (m_bMenuUp == false && n_bActivate != 0)
	{
		m_bBlockInput = true;
		CEGUI::FrameWindow* background = (CEGUI::FrameWindow*)createWidget("OgreTray/AltStaticImage",
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f), "background");
		background->setProperty(background->RiseOnClickEnabledPropertyName, "false");
		CEGUI::FrameWindow* menuFrame = static_cast<CEGUI::FrameWindow*>(createWidget("OgreTray/FrameWindow",
			glm::vec4(0.4f, 0.3f, 0.2f, 0.34f), glm::vec4(0.0f), "menuFrame"));
		menuFrame->setDisabled(true);

		CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.32f, 0.18f, 0.05f), glm::vec4(0.0f), "Return"));
		button->setText("Return to Game");
		button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameGUI::onReturnToGame, this));

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.37f, 0.18f, 0.05f), glm::vec4(0.0f), "Settings"));
		button->setText("Settings");
		button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Control::Settings, &(*m_Control)));

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.42f, 0.18f, 0.05f), glm::vec4(0.0f), "Load"));
		button->setText("Load Game");

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.47f, 0.18f, 0.05f), glm::vec4(0.0f), "Save"));
		button->setText("Save Game");

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.52f, 0.18f, 0.05f), glm::vec4(0.0f), "QuitToMain"));
		button->setText("Quit To Main Menu");
		button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Control::mainMenu, &(*m_Control)));

		button = static_cast<CEGUI::PushButton*>(createWidget("OgreTray/Button",
			glm::vec4(0.41f, 0.57f, 0.18f, 0.05f), glm::vec4(0.0f), "QuitToDesktop"));
		button->setText("Quit To Desktop");
		button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameGUI::onExitToDesktop, this));

		m_bMenuUp = true;
	}
	else if (n_bActivate != 1) {
		m_bBlockInput = false;
		destroyWidget("menuFrame");
		destroyWidget("Return");
		destroyWidget("Settings");
		destroyWidget("Load");
		destroyWidget("Save");
		destroyWidget("QuitToMain");
		destroyWidget("QuitToDesktop");
		destroyWidget("background");
		m_bMenuUp = false;
	}

	return m_bMenuUp;
}

bool GameGUI::mouseAction(int button, int action)
{
	injectMouseAction(button, action);
	return true;
}

bool GameGUI::keyAction(int key, int scancode, int action)
{
	if (m_bBlockInput)
	{
		return false;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		if (toggleGameMenu(2))
		{
			m_Control->setPause(true);
		}
		else
		{
			m_Control->setPause(false);
		}

	if (checkScreenMoveKey(key, action))
	{
		return true;
	}
}

bool GameGUI::onSelectUnit(const CEGUI::EventArgs& e)
{
	const CEGUI::WindowEventArgs& we =
		static_cast<const CEGUI::WindowEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();
	string id = senderID.c_str();
	id = id.substr(10);
	std::istringstream buffer(id);
	int value;
	buffer >> value;
	m_model->selectUnit(value);
	return true;
}

bool GameGUI::onReturnToGame(const CEGUI::EventArgs& e)
{
	toggleGameMenu(false);
	return true;
}

bool GameGUI::onExitToDesktop(const CEGUI::EventArgs& e)
{
	CEGUI::FrameWindow* background = (CEGUI::FrameWindow*)createWidget("OgreTray/AltStaticImage",
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f), "popupbackground");
	background->setProperty(background->RiseOnClickEnabledPropertyName, "false");
	CEGUI::FrameWindow* popupMenu = (CEGUI::FrameWindow*)createWidget("OgreTray/StaticImage",
		glm::vec4(0.4f, 0.3f, 0.2f, 0.15f), glm::vec4(0.0f), "popup");
	popupMenu->setDisabled(true);
	CEGUI::Window* statictext = (CEGUI::Window*)createWidget("OgreTray/StaticText",
		glm::vec4(0.41f, 0.31f, 0.18f, 0.05f), glm::vec4(0.0f), "text");
	statictext->setText("Really exit to desktop?");
	statictext->setProperty("FrameEnabled", "false");
	statictext->setProperty("BackgroundEnabled", "false");
	CEGUI::PushButton* quit = (CEGUI::PushButton*)createWidget("OgreTray/Button",
		glm::vec4(0.41f, 0.38f, 0.08f, 0.05f), glm::vec4(0.0f), "quit");
	quit->setText("Quit");
	quit->subscribeEvent("Clicked", CEGUI::Event::Subscriber(&Control::exitToDesktop, &(*m_Control)));
	CEGUI::PushButton* cancel = (CEGUI::PushButton*)createWidget("OgreTray/Button",
		glm::vec4(0.51f, 0.38f, 0.08f, 0.05f), glm::vec4(0.0f), "cancel");
	cancel->setText("Cancel");
	cancel->subscribeEvent("Clicked", CEGUI::Event::Subscriber(&GameGUI::quitCancel, this));
	return true;
}

bool GameGUI::quitCancel(const CEGUI::EventArgs& e)
{
	destroyWidget("popupbackground");
	destroyWidget("popup");
	destroyWidget("quit");
	destroyWidget("cancel");
	destroyWidget("text");
	return true;
}

void GameGUI::setupGL()
{
	float ratio;
	glfwGetFramebufferSize(m_window.get(), &m_width, &m_height);

	perspectiveSetup();
	ratio = m_width / (float)m_height;
	InitGL();
}

void GameGUI::perspectiveSetup() {
	float ratio;
	ratio = m_width / (float)m_height;
	glViewport(0, 0, m_width, m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width / 10, width / 10, -height / 10, height / 10, -350, 350);
	perspectiveGL(m_viewZoomFactor / m_zoomFactor, (GLfloat)m_width / (GLfloat)m_height, 0.1f, 350.0f);
	glMatrixMode(GL_MODELVIEW);
}

void GameGUI::InitGL()
{

	//CEGUI::OpenGL3Renderer& gui = CEGUI::OpenGL3Renderer::bootstrapSystem();
	m_texture.push_back(m_txtrLoader->retrieveMapTexture()[0]);
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

void GameGUI::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

// Determine dependencies for the topological graph sort
void GameGUI::TopologicalGraphSort(std::vector<std::shared_ptr<GameObject>>& nGameObjects)
{
	std::vector<std::shared_ptr<GameObject>> tmpGameObjects;
	int test = nGameObjects.size();
	for (int a = 0; a < nGameObjects.size(); a++)
	{
		

		nGameObjects[a]->clearGameObjectBehind();
		if (nGameObjects[a]->getZHeight() >= 2)
		{
			tmpGameObjects.push_back(std::shared_ptr<GameObject>(nGameObjects[a]));

		}
		else
		{
			nGameObjects[a]->setIsoDepth(0);
		}

	}
	int behindIndex;
	const int isoGameObjectsLength = tmpGameObjects.size();
	for (int i = 0; i < isoGameObjectsLength; ++i)
	{

		GameObject& a = *tmpGameObjects[i];
		behindIndex = 0;

		for (int j = 0; j < isoGameObjectsLength; ++j)
		{
			if (i != j)
			{
				GameObject& b = *tmpGameObjects[j];

				if (b.getIsoLocation().x== 41 && b.getIsoLocation().y == 63)
				{
					int trap = 1;
				}
				if (b.getMin().x < a.getMax().x && b.getMin().y < a.getMax().y && b.getMin().z <= a.getMax().z)
				{
					if (b.getMin().z == a.getMax().z && (b.getMax().y > a.getMax().x))
						continue;
					a.addGameObjectBehind(tmpGameObjects[j]);
				}
			}
		}

		a.setIsoVisitedFlag(false);
	}
	int depthTest = 1;
	for (int i = 0; i < tmpGameObjects.size(); ++i)
	{
		tmpGameObjects[i]->visitNode(depthTest);
	}

}



bool GameGUI::checkScreenMoveKey(int key, int action)
{
	// xMove = 0 yMove = 1
	int xyMovement = -1;
	int direction = 0;
	switch (key)
	{
	case GLFW_KEY_A:
		xyMovement = 0;
		direction = 1;
		break;
	case GLFW_KEY_D:
		xyMovement = 0;
		direction = -1;
		break;
	case GLFW_KEY_W:
		xyMovement = 1;
		direction = -1;
		break;
	case GLFW_KEY_S:
		xyMovement = 1;
		direction = 1;
		break;
	default:
		return false;
	}
	if (action == GLFW_PRESS)
	{
		if (xyMovement)
		{
			setYMovement(direction);
		}
		else
		{
			setXMovement(direction);
		}
	}
	else if (action == GLFW_RELEASE)
	{
		if (xyMovement)
		{
			if (getYMovement() != -(direction)) {
				setYMovement(0);
			}
		}
		else
		{
			if (getXMovement() != -(direction)) {
				setXMovement(0);
			}
		}
	}
	return true;
}

GameGUI::~GameGUI()
{
}
 