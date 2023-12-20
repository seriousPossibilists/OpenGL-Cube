#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"



const unsigned int width = 800;
const unsigned int height = 800;


// std::vector<GLfloat> vertices {
// -0.5f, 0.0f,  0.5f,     1.0f, 0.70f, 0.4f,	
// 	-0.5f, 0.0f, -0.5f,     0.83f, 0.7f, 1.0f,	
// 	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	
// 	 0.5f, 0.0f,  0.5f,     1.0f, 0.70f, 1.0f,	
// 	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	
//     };
// std::vector<GLuint> indices = {
// 	        //Top
// 	0, 1, 2,
// 	0, 2, 3,
// 	0, 1, 4,
// 	1, 2, 4,
// 	2, 3, 4,
// 	3, 0, 4
// };

std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

void genPlane(int div, float width, std::vector<int> perlin)
{
	float triangleSide = width / div;
	for(int row = 0; row < div + 1; row++)
	{
		for(int col = 0; col < div + 1; col++)
		{
			glm::vec3 crntVec = glm::vec3(col * triangleSide, (perlin[row / 1000]), row * -triangleSide);
			vertices.push_back((float)crntVec.x);
			vertices.push_back((float)crntVec.y);
			vertices.push_back((float)crntVec.z);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

		}
	}
};

void genPlaneIndices(int div)
{
	for(int row = 0; row < div; row++)
	{
		for(int col = 0; col < div; col++)
		{
			int index = row * (div + 1) + col;

			indices.push_back(index);
			indices.push_back(index + (div + 1) + 1);
			indices.push_back(index + (div + 1));

			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + (div + 1) + 1);


		}
	}
};

std::vector<int> openPerlin()
{
	std::ifstream fin("perlin/perlin_noise.dat");
    int num;
    std::vector<int> vec;
    
    while (fin >> num){
		std::cout << num << std::endl;
        vec.push_back(num);
	}
	
	return vec;
}

int main()
{
	// Initialize GLFW
	glfwInit();
	std::vector<int> data = openPerlin();
	genPlane(6, 1, data);
	genPlaneIndices(6);
	std::cout << data[0] << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("shader/triangle.vert", "shader/triangle.frag");


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(&vertices, vertices.size());
	EBO EBO1(&indices, indices.size());

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	
	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(vertices[0], vertices[1], vertices[3]));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(1.0f, 0.13f, 0.17f, 1.0f);
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