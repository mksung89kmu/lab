//#define  FREEGLUT_LIB_PRAGMAS  0

#include <iostream>
#include <GL/gl3w.h>
#include <string>
#include "Loader.h"
#include "Program.h"

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void draw();
private:

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	GLuint ibo_cube;
	GLuint vbo_vertex;
	
	std::unique_ptr<ShaderProgram> shaderProgram;
	std::unique_ptr<Program> shaderProgram_new;

	int m_width;
	int m_height;
	
	void setupBuffer();

};
