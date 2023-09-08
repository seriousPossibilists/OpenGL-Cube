#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"



const unsigned int width = 800;
const unsigned int height = 800;


std::vector<GLfloat> vertices = {         
		-1.0f, -1.0f,  0.5f, 1.0f, 1.0f, 1.0f,//0
         1.0f, -1.0f,  0.5f, 1.0f, 1.0f, 1.0f,//1
        -1.0f,  1.0f,  0.5f, 1.0f, 1.0f, 1.0f,//2
         1.0f,  1.0f,  0.5f, 1.0f, 1.0f, 1.0f,//3
        -1.0f, -1.0f, -0.5f, 1.0f, 1.0f, 1.0f,//4
         1.0f, -1.0f, -0.5f, 1.0f, 1.0f, 1.0f,//5
        -1.0f,  1.0f, -0.5f, 1.0f, 1.0f, 1.0f,//6
         1.0f,  1.0f, -0.5f,  1.0f, 1.0f, 1.0f,//7};
};
std::vector<GLfloat> indices = {
	        //Top
        2, 6, 7,
        2, 3, 7,

        //Bottom
        0, 4, 5,
        0, 1, 5,

        //Left
        0, 2, 6,
        0, 4, 6,

        //Right
        1, 3, 7,
        1, 5, 7,

        //Front
        0, 2, 3,
        0, 1, 3,

        //Back
        4, 6, 7,
        4, 5, 7
};


// void genPlane(int div, float width)
// {
// 	float triangleSide = width / div;
// 	for(int row = 0; row < div + 1; row++)
// 	{
// 		for(int col = 0; col < div + 1; col++)
// 		{
// 			glm::vec3 crntVec = glm::vec3(col * triangleSide, 0.0f, row * -triangleSide);
// 			vertices.push_back(crntVec.x);
// 			vertices.push_back(crntVec.y);
// 			vertices.push_back(crntVec.z);

// 		}
// 	}
// };

// void genPlaneIndices(int div)
// {
// 	for(int row = 0; row < div + 1; row++)
// 	{
// 		for(int col = 0; col < div + 1; col++)
// 		{
// 			int index = row * (div + 1) + col;

// 			indices.push_back(index);
// 			indices.push_back(index + (div + 1) + 1);
// 			indices.push_back(index + (div + 1));

// 			indices.push_back(index);
// 			indices.push_back(index + (div + 1));
// 			indices.push_back(index + (div + 1) + 1);


// 		}
// 	}
// };



int main()
{
	// Initialize GLFW
	glfwInit();;
	std::cout << vertices[25];

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("shader/triangle.vert", "shader/triangle.frag");


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(&vertices, vertices.size());
	EBO EBO1(&indices, indices.size());

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	
	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Binds texture so that is appears in rendering
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, (void*)0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}