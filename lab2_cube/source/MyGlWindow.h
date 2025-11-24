//#define  FREEGLUT_LIB_PRAGMAS  0

#pragma warning(push)
#pragma warning(disable:4311)		// convert void* to long
#pragma warning(disable:4312)		// convert long to void*

#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include <stack>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"


#include "cube.h"
#include "Viewer.h"
#include "Program.h"


#pragma warning(pop)


class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	~MyGlWindow();
	void draw();
	void setSize(int w, int h) { m_width = w; m_height = h; }
	void setAspect(float r) { m_viewer->setAspectRatio(r); }

	//fix it to use smart pointers
	std::unique_ptr<Viewer> m_viewer;
private:
	int m_width;
	int m_height;
	void initialize();

	std::unique_ptr<colorCube> m_cube;

};
