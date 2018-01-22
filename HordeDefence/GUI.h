#pragma once

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "textureLoader.h"
#include "Model.h"

#include "ImageButton.h"

class Control;
class GUI {
public:

	GUI();
	~GUI();
	// Abstract class, use method to load gui in subclass
	virtual void loadGUI() = 0;

	void init(const std::string& resourceDirectory);
	void destroy();

	virtual void start() = 0;
	virtual void update() = 0;
	virtual void close() = 0;

	void draw();

	void setMouseCursor(const std::string imageFile);
	void showMouseCursor();
	void hideMouseCursor();
	void setMousePosition(double xPos, double yPos);

	virtual bool keyAction(int key, int scancode, int action) = 0;
	virtual bool mouseAction(int button, int action) = 0;

	bool injectMouseAction(int button, int action);

	CEGUI::MouseButton GlfwToCeguiButton(int glfwButton);

	void loadScheme(const std::string& schemeFile);
	void setFont(const std::string& fontFile);
	CEGUI::Window* createWidget(const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = "");
	CEGUI::Texture* createTexture(string texture);
	static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);
	void destroyWidget(const std::string& name);
	shared_ptr<ImageButton> GUI::createImageButton(const glm::vec4& vec, const string& buttonName, const string& texturePath);

	// Getters and Setters
	static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; }
	const CEGUI::GUIContext* getContext() { return m_context; }

	void setControl(std::shared_ptr<Control> n_ctrl);
	void setModel(std::shared_ptr<Model> n_model) { m_model = std::shared_ptr<Model>(n_model); }
	void setWindow(std::shared_ptr<GLFWwindow> n_window) { m_window = std::shared_ptr<GLFWwindow>(n_window); }
	void setTextureLoader(std::shared_ptr<textureLoader> n_loader) { m_txtrLoader = std::shared_ptr<textureLoader>(n_loader); }

	// Getters and Setters
	float getXScreenLoc() const { return m_xScreenLoc; }
	float getYScreenLoc() const { return m_yScreenLoc; }
	float getZScreenLoc() const { return m_zScreenLoc; }
	void setZoomFactor(float inZoom) { if (inZoom >= 6 && inZoom <= 12) { m_zoomFactor = inZoom; } }
	void increaseZoomFactor(float inZoomIncrease);
	void decreaseZoomFactor(float inZoomDecrease);
	void changeXScreenLoc(float inXScreenLoc);
	void changeYScreenLoc(float inYScreenLoc);
	void setYMovement(float inYMovement) { m_yIncreasing = inYMovement; }
	void setXMovement(float inXMovement) { m_xIncreasing = inXMovement; }
	float getXMovement() { return m_xIncreasing; }
	float getYMovement() { return m_yIncreasing; }

protected:
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = nullptr;
	CEGUI::Window* m_root = nullptr;

	std::shared_ptr<Control> m_Control;

	std::shared_ptr<Model> m_model;
	std::shared_ptr<GLFWwindow> m_window;

	std::vector<GLuint>	m_texture;			// Storage For 1 Texture
	std::shared_ptr<textureLoader> m_txtrLoader;

	int m_width, m_height;
	float m_viewZoomFactor;
	float m_zoomFactor;
	float m_yScreenLoc;
	float m_xScreenLoc;
	float m_zScreenLoc;
	float m_xIncreasing, m_yIncreasing;
	std::map<string, CEGUI::Texture*> mTextures;
	int mImageCount;
	virtual void perspectiveSetup() {};

};