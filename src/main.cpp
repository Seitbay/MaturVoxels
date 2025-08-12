#include"Model.h"
#include<chrono>
#include <thread>

const unsigned int width = 800;
const unsigned int height = 800;

const int targetFPS = 60;
const float targetFrameTime = 1.0f / targetFPS;


using namespace std;


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

	//------------------DefaultShader-----------------------
	Shader shaderProgram("assets/shaders/default.vert", "assets/shaders/default.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	std::cout << "Program here " << std::endl;

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	std::cout << "Program here " << std::endl;

	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	double prevTime = glfwGetTime();
	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	Model sword("assets/Models/sword/scene.gltf");

	while (!glfwWindowShouldClose(window)) {
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		sword.Draw(shaderProgram, camera);

		// FPS limiter
		float frameTime = glfwGetTime() - currentFrameTime;
		if (frameTime < targetFrameTime)
		{
			std::this_thread::sleep_for(std::chrono::duration<float>(targetFrameTime - frameTime));
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	std::cin.get();
	return 0;
}