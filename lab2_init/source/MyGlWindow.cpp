
#define GLM_ENABLE_EXPERIMENTAL 
#include "MyGlWindow.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

MyGlWindow::MyGlWindow(int w, int h)

//==========================================================================
{
	m_width = w;
	m_height = h;

	setupBuffer();
	
}




void MyGlWindow::setupBuffer()
{
	

	shaderProgram = std::make_unique<ShaderProgram>();
	
	//load shaders
	shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");

	

	
}

void MyGlWindow::draw(void)
{
	


}
