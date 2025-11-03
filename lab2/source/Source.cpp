
//#define GLFW_INCLUDE_GLU

#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


#include "MyGlWindow.h"

MyGlWindow * win;

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}




int main(void)
{


	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{

	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	int width = 800;
	int height = 800;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, u8"³ªÀÇ ¿ÀÇÂÁö¿¤", NULL, NULL);


	if (!window)
	{
		glfwTerminate();
		return -1;
	}



	glfwMakeContextCurrent(window);


	/* Make the window's context current */
 	if (gl3wInit()) {
// 		fprintf(stderr, "failed to initialize OpenGL\n");
 		return -1;
 	}

//	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
// 
// 	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
// 		glGetString(GL_SHADING_LANGUAGE_VERSION));


	glfwSwapInterval(1);  //enable vsync

	// Setup ImGui binding
	


	win = new MyGlWindow(width, height);

	bool show_test_window = true;
	bool show_another_window = false;
	


	double previousTime = glfwGetTime();
	int frameCount = 0;
	double lastTime = 0;


	glfwSetKeyCallback(window, key_callback);


	glfwSetWindowTitle(window, u8"³ªÀÇ ¿ÀÇÂÁö¿¤");

	

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

	
		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT);


		win->draw();

		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		//glfwWaitEvents();

		

	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}


