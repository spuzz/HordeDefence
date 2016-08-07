#pragma once

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <memory>s

class Control;
class GUI {
public:

	// Abstract class, use method to load gui in subclass
	virtual void loadGUI() = 0;

	void init(const std::string& resourceDirectory);
	void destroy();

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
	static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);
	void destroyWidget(const std::string& name);

	// Getters and Setters
	static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; }
	const CEGUI::GUIContext* getContext() { return m_context; }

	void setControl(std::shared_ptr<Control> n_ctrl);

protected:
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = nullptr;
	CEGUI::Window* m_root = nullptr;

	std::shared_ptr<Control> m_Control;
};