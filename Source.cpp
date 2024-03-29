﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iostream>

#include "cylinder.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 4.0f);


glm::vec3 lightPos1(-3.0f, 1.0f, 1.0f);
glm::vec3 lightColor1(0.0f, 1.0f, 0.0f);

float xlight = 1.2f, ylight = 3.0f, zlight = 2.0f;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.5f, -1.0f); //>>>>>>>>>>>>>>>>>>>
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "7-1 Final Project_GomesDaCosta", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("shaderfiles/7.3.camera.vs", "shaderfiles/7.3.camera.fs");
	Shader lightCubeShader("shaderfiles//6.light_cube.vs", "shaderfiles//6.light_cube.fs");
	Shader lightingShader("shaderfiles/2.2.basic_lighting.vs", "shaderfiles/2.2.basic_lighting.fs");

	//configure the light's VAO 
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//Triangle 1
		 -1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		  1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		 //Triangle 2
		  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		 -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

		 //Triangle 3
		 -1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 -1.0f, 1.0f, -0.5f,   1.0f, 0.0f,
		  1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		  // Triangle 4
		  1.0f, 1.0f, -0.5f,   1.0f, 0.0f,
		  1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
		  -1.0f, 1.0f, -0.5f,  1.0f, 0.0f,

		  //Triangle 5
		  -1.0f, 1.0f, -0.5f,   0.0f, 1.0f,
		   1.0f, 1.0f, -0.5f,   0.0f, 1.0f,
		  -1.0f, 0.0f, -0.5f,   0.0f, 1.0f,
		  //Triangle 6
		  1.0f, 1.0f, -0.5f,   0.0f, 1.0f,
		   1.0f, 0.0f, -0.5f,  0.0f, 1.0f,
		  -1.0f, 0.0f, -0.5f,  0.0f, 1.0f,

		  // Triangle 7
		  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
		  -1.0f, 0.0f, -0.5f,  0.0f, 0.0f,
		  // Triangle 8
		  1.0f, 0.0f, -0.5f,   0.0f, 0.0f,
		  -1.0f, 0.0f, -0.5f,  0.0f, 0.0f,
		  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,

		  // Triangle 9
		  -1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		  -1.0f, 1.0f, -0.5f,  1.0f, 1.0f,
		  //Triangle 10
		  -1.0f, 1.0f, -0.5f,   1.0f, 1.0f,
		  -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
		  -1.0f, 0.0f, -0.5f,   1.0f, 1.0f,

		  // Triangle 11
		  1.0f, 1.0f, 0.0f,   0.5f, 0.5f,
		  1.0f, 0.0f, 0.0f,   0.5f, 0.5f,
		  1.0f, 1.0f, -0.5f,  0.5f, 0.5f,
		  // Triangle 12
		  1.0f, 1.0f, -0.5f,   0.5f, 0.5f,
		  1.0f, 0.0f, 0.0f,    0.5f, 0.5f,
		  1.0f, 0.0f, -0.5f,   0.5f, 0.5f,

		  //background
	   -5.0f,  5.0f, -5.0f, 0.0f, 0.0f,
	   5.5f,  5.0f, -5.0f,  1.0f, 0.0f,
	  -5.0f,  0.0f, -5.0f,  0.0f, 1.0f,

	  -5.0f,  0.0f, -5.0f,  0.0f, 1.0f,
	   5.5f,  0.0f, -5.0f,  1.0f, 1.0f,
	   5.5f,  5.0f, -5.0f,  1.0f, 0.0f,


	   -5.0f,  0.0f, -5.0f,  1.0f, 1.0f,
		5.5f,  0.0f, -5.0f,  0.0f, 1.0f,
	   -5.0f,  0.0f,  5.0f,  1.0, 0.0f,

	   -5.0f,  0.0f,  5.0f,  0.0, 0.0f,
		5.0f,  0.0f,  5.0f,  1.0f, 0.0f,
		5.5f,  0.0f, -5.0f,  1.0f, 1.0f
	};

	unsigned int VBO, VAO;
	unsigned int VBO2, VAO2;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2, texture3;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("images/background.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/beads.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 3
   // ---------
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("images/headset.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setInt("texture3", 2);

	glm::mat4 model;
	float angle;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		lightPos[0] = xlight;
		lightPos[1] = ylight;
		lightPos[2] = zlight;

		lightPos1[0] = xlight;
		lightPos1[1] = ylight;
		lightPos1[2] = zlight;

		// render
		// ------

		//Texture1
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);  // sets the background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		// render boxes
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 36, 48);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO2);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-3.0f, 1.0f, -1.0f));
		ourShader.setMat4("model", model);

		static_meshes_3D::Cylinder C(0.5, 12, 2, true, true, true);
		C.render();

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection1 = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection1", projection1);

		// camera/view transformation
		glm::mat4 view1 = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view1", view1);

		// render boxes
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 12);


		//draw lamp object
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); //smaller cube
		lightCubeShader.setMat4("model", model);

		static_meshes_3D::Cylinder C2(1.6, 30, 1.5, true, true, true);
		C2.render();

		glBindVertexArray(lightCubeVAO);

		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}