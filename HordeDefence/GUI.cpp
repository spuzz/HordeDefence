#include "GUI.h"
#include "Control.h"
CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;

void GUI::init(const std::string& resourceDirectory) {
	// Check if the renderer and system were not already initialized
	if (m_renderer == nullptr) {
		m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();

		CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
		rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
		rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
		rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
		rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/");
		rp->setResourceGroupDirectory("looknfeels", resourceDirectory + "/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");

		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
	}

	m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());
	m_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
	m_context->setRootWindow(m_root);
}

void GUI::destroy() {
	CEGUI::System::getSingleton().destroyGUIContext(*m_context);
}

void GUI::draw() {
	m_renderer->beginRendering();
	m_context->draw();
	m_renderer->endRendering();
	glDisable(GL_SCISSOR_TEST);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
}

void GUI::loadScheme(const std::string& schemeFile) {
	CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
}

CEGUI::Window* GUI::createWidget(const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name /*= ""*/) {
	CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	m_root->addChild(newWindow);
	setWidgetDestRect(newWindow, destRectPerc, destRectPix);
	return newWindow;
}

void GUI::destroyWidget(const std::string& name) {
	m_root->getChild(name)->destroy();
}

void GUI::setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix) {
	widget->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
	widget->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
}

void GUI::setFont(const std::string& fontFile) {
	CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
	m_context->setDefaultFont(fontFile);
}

void GUI::setMouseCursor(const std::string imageFile)
{
	m_context->getMouseCursor().setDefaultImage(imageFile);
}
void GUI::showMouseCursor()
{
	m_context->getMouseCursor().show();
	
}

void GUI::hideMouseCursor()
{
	m_context->getMouseCursor().hide();
}

void GUI::setMousePosition(double xPos, double yPos)
{
	m_context->injectMousePosition(xPos, yPos);
}

bool GUI::injectMouseAction(int button, int action)
{
	if (action == GLFW_PRESS)
	{
		m_context->injectMouseButtonDown(CEGUI::LeftButton);
	}
	if (action == GLFW_RELEASE)
	{
		m_context->injectMouseButtonUp(CEGUI::LeftButton);
	}
	return true;
}

CEGUI::MouseButton GUI::GlfwToCeguiButton(int glfwButton)
{
	switch (glfwButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT: return CEGUI::LeftButton;
	case GLFW_MOUSE_BUTTON_RIGHT: return CEGUI::RightButton;
	case GLFW_MOUSE_BUTTON_MIDDLE: return CEGUI::MiddleButton;
	default: return CEGUI::NoButton;
	}
}

void GUI::setControl(shared_ptr<Control> n_ctrl)
{
	m_Control = shared_ptr<Control>(n_ctrl);
}