#include"Model.h"
#include"PointLight.h"
#include<chrono>
#include <thread>

const unsigned int width = 800;
const unsigned int height = 800;

const int targetFPS = 60;
const float targetFrameTime = 1.0f / targetFPS;


using namespace std;


float rectangleVertices[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

float axisVertices[] = {
    // X (red)
    0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

    // Y (green)
    0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

    // Z (blue)
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
};



int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL_Test", NULL, NULL);
	if (window == NULL) {
		
		cout << "Failed to create GLFWwindow\n";
		glfwTerminate();
		std::cin.get();

		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	Shader lightShader("assets/shaders/light.vert", "assets/shaders/light.frag");

	//------------------DefaultShader-----------------------
	Shader shaderProgram("assets/shaders/default.vert", "assets/shaders/default.frag");
	Shader framebufferProgram("assets/shaders/framebuffer.vert", "assets/shaders/framebuffer.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(1.6f, 1.8f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	double prevTime = glfwGetTime();
	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	Model sword("assets/Models/sword/scene.gltf");
	Model light("assets/Models/LightSource/LightSource.gltf");

	PointLight pointLight(lightColor, lightPos);


	// ========== temporary VAO for axis ================
	unsigned int axisVAO, axisVBO;
	glGenVertexArrays(1, &axisVAO);
	glGenBuffers(1, &axisVBO);

	glBindVertexArray(axisVAO);

	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

	// позиции
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// цвета
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader axisShader("assets/shaders/axis.vert", "assets/shaders/axis.frag");
	// ==================================================

glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		// FPS limiter
		float frameTime = glfwGetTime() - currentFrameTime;
		if (frameTime < targetFrameTime)
		{
			std::this_thread::sleep_for(std::chrono::duration<float>(targetFrameTime - frameTime));
		}
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		camera.Inputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		pointLight.Draw(lightShader, camera);	
		sword.Draw(shaderProgram, camera);
		light.Draw(shaderProgram, camera);
		
		// Gismos todo create class for it and more features
		{
			glDisable(GL_DEPTH_TEST);
			axisShader.Activate();
			camera.Matrix(axisShader, "camMatrix");
			glBindVertexArray(axisVAO);
			glDrawArrays(GL_LINES, 0, 6);
			glEnable(GL_DEPTH_TEST);
		}
		//=================================================


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	lightShader.Delete();
	axisShader.Delete();
	glDeleteVertexArrays(1, &axisVAO);
	glDeleteBuffers(1, &axisVBO);

	glfwDestroyWindow(window);

	glfwTerminate();
	std::cin.get();
	return 0;
}