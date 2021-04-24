#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
GLsizei screen_width;
GLsizei screen_height;

void init();
void update();
void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";

    screen_width = 800;
    screen_height = 800;

    window = glfwCreateWindow(screen_width, screen_height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();

    init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


    return 0;
}


void init()
{
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });
    
    
}


void update()
{

}

void draw_rectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    glPushMatrix();  //Make sure our transformations don't affect any other transformations in other code
    glTranslatef(x - (width / 2), y - (height / 2), 0.0f);  //Translate rectangle to its assigned x and y position
    //Put other transformations here
    glBegin(GL_QUADS);   //We want to draw a quad, i.e. shape with four sides 
    glVertex2f(0, 0);            //Draw the four corners of the rectangle
    glVertex2f(0, height);
    glVertex2f(width, height);
    glVertex2f(width, 0);
    glEnd();
    glPopMatrix();
}

void draw()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);  //Set the cleared screen colour to black
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT); //Clear the screen and depth buffer
    draw_rectangle(-1, -1, 0.1, 0.1);
}
