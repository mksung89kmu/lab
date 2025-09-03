//#define  FREEGLUT_LIB_PRAGMAS  0

#include <iostream>
#include <GL/gl3w.h>
#include <string>
#include "Loader.h"


class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void draw();
private:

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	GLuint vbo_vertex;
	
	ShaderProgram* shaderProgram;

	int m_width;
	int m_height;
	
	void setupBuffer();

};
