#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "shader.h"
#include "tree.h"
#include "camera.h"
#include "model.h"
#include "wall.h"
#include "graph.h"  //NEWLY ADDED
using namespace GraphNameSpace;  //NEWLY ADDED

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawSegment(glm::vec3 point1, glm::vec3 point2, Shader* shader, glm::mat4 view, glm::mat4 projection);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);
void InitCrashCube(Wall *CB);   //NEWLY ADDED
unsigned int segmentVAO();
void drawSegment(glm::vec3 p1, glm::vec3 p2, Shader* shader, unsigned int VAO, glm::mat4 view, glm::mat4 projection);
void InitMap();

float allowhight = 2.1;
const int cubelength = 22;
glm::vec3 posG = glm::vec3(2.5, 0, -116);
float gardenradius = 14;
Wall cube[cubelength];
int reset = 0;


glm::vec3 HeroPosition = glm::vec3(0, 0, 0);
float HeroFront = 0;
float HeroRotateSpeed = 20;

Camera camera(HeroPosition, HeroPosition + glm::vec3(0.0f, 0.25f, -0.20f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
Type destination("10: Building_A", glm::vec3(-82, 0, -106));

Graph Mymap(WEIGHTED);  //NEWLY ADDED


bool RoadSearchSwitch = false;
bool lighton = 0;
bool day = 1;
bool spoton = 0;
bool walk = 0;

const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1600;


int main()
{
	// 环境初始化部分 Environment Initiate
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mapping", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 启动回调函数：键盘，鼠标，滚轮等 Set on the callbacks: keyboard, mouse, scroll
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// 加载着色器  Load Shader
	Shader skyboxShader("../Shaders/skybox.vs", "../Shaders/skybox.fs");
	//Shader modelShader("../Shaders/model_loading.vs", "../Shaders/model_loading.fs");
	//std::cout << "modelShader success!" << std::endl;
	Shader lineSegmentShader("../Shaders/lineSeg.vs", "../Shaders/lineSeg.fs");
	//std::cout << "lineSegmentShader success!" << std::endl;

	Shader modelShader_on("../Shaders/model_on.vs", "../Shaders/model_on.fs");
	Shader modelShader_off("../Shaders/model_off.vs", "../Shaders/model_off.fs");
	Shader lightShader("../Shaders/light.vs", "../Shaders/light.fs");

	//Init Map
	InitMap();

	// 加载模型 Load Model
	//Model ourModel("../Objects/nanosuit/nanosuit.obj");
	Model ourModel("../Objects/man/man.obj");//新的人物
	Model ground("../Objects/ground/ground.obj");//场景模型
	Model A("../Objects/A/A.obj");
	Model B("../Objects/B/B.obj");
	Model C("../Objects/C/C.obj");
	Model F("../Objects/F/F.obj");
	Model xingkong("../Objects/xingkong/xingkong.obj");
	Model qiao("../Objects/qiao/qiao.obj");
	Model cat("../Objects/cat/cat.obj");
	Model D("../Objects/D/D.obj");
	Model H("../Objects/H/H.obj");
	Model E("../Objects/E/E.obj");
	Model G("../Objects/G/G.obj");
	Model luzhang("../Objects/luzhang/luzhang.obj");
	Model lamp("../Objects/lamp/lamp.obj");
	Model bicycle("../Objects/bicycle/bicycle.obj");
	

	// 加载碰撞盒
	InitCrashCube(cube);   //NEWLY ADDED





	// 加载天空盒 Load Skybox
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	
	std::vector<std::string> faces_day
	{
		"..\\Textures\\SkyBox\\Background_Right.jpg",
		"..\\Textures\\SkyBox\\Background_Left.jpg",
		"..\\Textures\\SkyBox\\Background_Sky.jpg",
		"..\\Textures\\SkyBox\\Background_Ground.jpg",
		"..\\Textures\\SkyBox\\Background_Forward.jpg",
		"..\\Textures\\SkyBox\\Background_Backward.jpg"
	};

	std::vector<std::string> faces_night
	{
		"..\\Textures\\SkyBox3\\Background_Right.jpg",
		"..\\Textures\\SkyBox3\\Background_Left.jpg",
		"..\\Textures\\SkyBox3\\Background_Sky.jpg",
		"..\\Textures\\SkyBox3\\Background_Ground.jpg",
		"..\\Textures\\SkyBox3\\Background_Forward.jpg",
		"..\\Textures\\SkyBox3\\Background_Backward.jpg"
	};

	unsigned int cubemapTexture_day = loadCubemap(faces_day);
	unsigned int cubemapTexture_night = loadCubemap(faces_night);
	glm::vec3 pointLightPositions[] = {
		glm::vec3(-10.0f,  15.0f,  -38.5f),
		glm::vec3(-10.0f,  15.0f,  -57.5f),
		glm::vec3(-10.0f,  15.0f,  -74.5f),
		glm::vec3(-10.0f,  15.0f,  -93.5f),
		glm::vec3(-12.0f,  15.0f,  -163.0f),
		glm::vec3(-12.0f,  15.0f,  -182.0f),
		glm::vec3(12.0f,  15.0f,  -38.5f),
		glm::vec3(12.0f,  15.0f,  -57.5f),
		glm::vec3(12.0f,  15.0f,  -74.5f),
		glm::vec3(12.0f,  15.0f,  -93.5f),
		glm::vec3(14.0f,  15.0f,  -163.0f),
		glm::vec3(14.0f,  15.0f,  -182.0f)
	};


	

	unsigned int segmentID = segmentVAO();

	// 主循环部分 MainLoop
	while (!glfwWindowShouldClose(window))
	{
		// 帧更新时间 Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		std::cout <<"分辨率："<< deltaTime << std::endl;
		lastFrame = currentFrame;

		// 检查回调情况，调用回调函数 Check and call events
		glfwPollEvents();
		Do_Movement();

		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyboxShader.use();
		skyboxShader.setInt("skybox", 0);


		// Create camera transformation
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

		Shader modelShader; //= modelShader_on;

		if (lighton)
		{
			modelShader = modelShader_on;
		}
		else
		{
			modelShader = modelShader_off;
		}

		modelShader.use();
		modelShader.setInt("material.diffuse", 0);
		modelShader.setInt("material.specular", 1);

		if (lighton&&day)
		{
			// directional light
			modelShader.setVec3("dirLight.direction", 0.4f, -0.5f, -1.0f);
			modelShader.setVec3("dirLight.ambient", 0.6f, 0.6f, 0.6f);
			modelShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
			modelShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);
			// point light 1
			modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
			modelShader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[0].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[0].constant", 1.0f);
			modelShader.setFloat("pointLights[0].linear", 0.045f);
			modelShader.setFloat("pointLights[0].quadratic", 0.0075f);
			// point light 2
			modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
			modelShader.setVec3("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[1].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[1].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[1].constant", 1.0f);
			modelShader.setFloat("pointLights[1].linear", 0.045f);
			modelShader.setFloat("pointLights[1].quadratic", 0.0075f);
			// point light 3
			modelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
			modelShader.setVec3("pointLights[2].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[2].constant", 1.0f);
			modelShader.setFloat("pointLights[2].linear", 0.045f);
			modelShader.setFloat("pointLights[2].quadratic", 0.0075f);
			// point light 4
			modelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
			modelShader.setVec3("pointLights[3].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[3].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[3].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[3].constant", 1.0f);
			modelShader.setFloat("pointLights[3].linear", 0.045f);
			modelShader.setFloat("pointLights[3].quadratic", 0.0075f);
			// point light 5
			modelShader.setVec3("pointLights[4].position", pointLightPositions[4]);
			modelShader.setVec3("pointLights[4].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[4].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[4].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[4].constant", 1.0f);
			modelShader.setFloat("pointLights[4].linear", 0.045f);
			modelShader.setFloat("pointLights[4].quadratic", 0.0075f);
			// point light 6
			modelShader.setVec3("pointLights[5].position", pointLightPositions[5]);
			modelShader.setVec3("pointLights[5].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[5].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[5].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[5].constant", 1.0f);
			modelShader.setFloat("pointLights[5].linear", 0.045f);
			modelShader.setFloat("pointLights[5].quadratic", 0.0075f);
			// point light 7
			modelShader.setVec3("pointLights[6].position", pointLightPositions[6]);
			modelShader.setVec3("pointLights[6].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[6].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[6].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[6].constant", 1.0f);
			modelShader.setFloat("pointLights[6].linear", 0.045f);
			modelShader.setFloat("pointLights[6].quadratic", 0.0075f);
			// point light 8
			modelShader.setVec3("pointLights[7].position", pointLightPositions[7]);
			modelShader.setVec3("pointLights[7].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[7].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[7].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[7].constant", 1.0f);
			modelShader.setFloat("pointLights[7].linear", 0.045f);
			modelShader.setFloat("pointLights[7].quadratic", 0.0075f);
			// point light 9
			modelShader.setVec3("pointLights[8].position", pointLightPositions[8]);
			modelShader.setVec3("pointLights[8].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[8].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[8].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[8].constant", 1.0f);
			modelShader.setFloat("pointLights[8].linear", 0.045f);
			modelShader.setFloat("pointLights[8].quadratic", 0.0075f);
			// point light 10
			modelShader.setVec3("pointLights[9].position", pointLightPositions[9]);
			modelShader.setVec3("pointLights[9].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[9].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[9].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[9].constant", 1.0f);
			modelShader.setFloat("pointLights[9].linear", 0.045f);
			modelShader.setFloat("pointLights[9].quadratic", 0.0075f);
			// point light 11
			modelShader.setVec3("pointLights[10].position", pointLightPositions[10]);
			modelShader.setVec3("pointLights[10].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[10].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[10].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[10].constant", 1.0f);
			modelShader.setFloat("pointLights[10].linear", 0.045f);
			modelShader.setFloat("pointLights[10].quadratic", 0.0075f);
			// point light 12
			modelShader.setVec3("pointLights[11].position", pointLightPositions[11]);
			modelShader.setVec3("pointLights[11].ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[11].diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("pointLights[11].specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("pointLights[11].constant", 1.0f);
			modelShader.setFloat("pointLights[11].linear", 0.045f);
			modelShader.setFloat("pointLights[11].quadratic", 0.0075f);
		}
		else
		{
			// directional light
			modelShader.setVec3("dirLight.direction", 0.4f, -0.5f, -1.0f);
			modelShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
			modelShader.setVec3("dirLight.diffuse", 0.1f, 0.1f, 0.1f);
			modelShader.setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);
			// point light 1
			modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
			modelShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[0].constant", 1.0f);
			modelShader.setFloat("pointLights[0].linear", 0.045f);
			modelShader.setFloat("pointLights[0].quadratic", 0.0075f);
			// point light 2
			modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
			modelShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[1].constant", 1.0f);
			modelShader.setFloat("pointLights[1].linear", 0.045f);
			modelShader.setFloat("pointLights[1].quadratic", 0.0075f);
			// point light 3
			modelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
			modelShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[2].constant", 1.0f);
			modelShader.setFloat("pointLights[2].linear", 0.045f);
			modelShader.setFloat("pointLights[2].quadratic", 0.0075f);
			// point light 4
			modelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
			modelShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[3].constant", 1.0f);
			modelShader.setFloat("pointLights[3].linear", 0.045f);
			modelShader.setFloat("pointLights[3].quadratic", 0.0075f);
			// point light 5
			modelShader.setVec3("pointLights[4].position", pointLightPositions[4]);
			modelShader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[4].constant", 1.0f);
			modelShader.setFloat("pointLights[4].linear", 0.045f);
			modelShader.setFloat("pointLights[4].quadratic", 0.0075f);
			// point light 6
			modelShader.setVec3("pointLights[5].position", pointLightPositions[5]);
			modelShader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[5].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[5].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[5].constant", 1.0f);
			modelShader.setFloat("pointLights[5].linear", 0.045f);
			modelShader.setFloat("pointLights[5].quadratic", 0.0075f);
			// point light 7
			modelShader.setVec3("pointLights[6].position", pointLightPositions[6]);
			modelShader.setVec3("pointLights[6].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[6].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[6].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[6].constant", 1.0f);
			modelShader.setFloat("pointLights[6].linear", 0.045f);
			modelShader.setFloat("pointLights[6].quadratic", 0.0075f);
			// point light 8
			modelShader.setVec3("pointLights[7].position", pointLightPositions[7]);
			modelShader.setVec3("pointLights[7].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[7].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[7].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[7].constant", 1.0f);
			modelShader.setFloat("pointLights[7].linear", 0.045f);
			modelShader.setFloat("pointLights[7].quadratic", 0.0075f);
			// point light 9
			modelShader.setVec3("pointLights[8].position", pointLightPositions[8]);
			modelShader.setVec3("pointLights[8].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[8].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[8].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[8].constant", 1.0f);
			modelShader.setFloat("pointLights[8].linear", 0.045f);
			modelShader.setFloat("pointLights[8].quadratic", 0.0075f);
			// point light 10
			modelShader.setVec3("pointLights[9].position", pointLightPositions[9]);
			modelShader.setVec3("pointLights[9].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[9].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[9].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[9].constant", 1.0f);
			modelShader.setFloat("pointLights[9].linear", 0.045f);
			modelShader.setFloat("pointLights[9].quadratic", 0.0075f);
			// point light 11
			modelShader.setVec3("pointLights[10].position", pointLightPositions[10]);
			modelShader.setVec3("pointLights[10].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[10].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[10].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[10].constant", 1.0f);
			modelShader.setFloat("pointLights[10].linear", 0.045f);
			modelShader.setFloat("pointLights[10].quadratic", 0.0075f);
			// point light 12
			modelShader.setVec3("pointLights[11].position", pointLightPositions[11]);
			modelShader.setVec3("pointLights[11].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[11].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[11].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[11].constant", 1.0f);
			modelShader.setFloat("pointLights[11].linear", 0.045f);
			modelShader.setFloat("pointLights[11].quadratic", 0.0075f);
		}

		if (lighton&&spoton)
		{
			// spotLight
			modelShader.setVec3("spotLight.position", camera.Target + glm::vec3(0.0f, 5.0f, -2.0f));
			modelShader.setVec3("spotLight.direction", camera.Front);
			modelShader.setVec3("spotLight.ambient", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("spotLight.constant", 1.0f);
			modelShader.setFloat("spotLight.linear", 0.022f);
			modelShader.setFloat("spotLight.quadratic", 0.0019f);
			modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		}
		else
		{
			// spotLight
			modelShader.setVec3("spotLight.position", camera.Target + glm::vec3(0.0f, 5.0f, -2.0f));
			modelShader.setVec3("spotLight.direction", camera.Front);
			modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
			modelShader.setFloat("spotLight.constant", 1.0f);
			modelShader.setFloat("spotLight.linear", 0.045f);
			modelShader.setFloat("spotLight.quadratic", 0.0075f);
			modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		}
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);



		glm::mat4 model = glm::mat4(1.0f);
		if (falling(camera.Target, cube, cubelength, 1, posG, gardenradius)) {
			reset++;
			camera.Target = camera.Target - glm::vec3(0, 1, 0);
			//camera.Position = camera.Position - glm::vec3(0, 1, 0);
		}
		else
			reset = 0;


		model = glm::translate(model, camera.Target); 
		model = glm::rotate(model, glm::radians(-camera.Yaw + 90), glm::vec3(0, 1, 0));
		if (walk)
			model = glm::scale(model, glm::vec3(0.10f, 0.15f, 0.10f));
		else
			model = glm::scale(model, glm::vec3(-0.10f, 0.15f, 0.10f));
		
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		ourModel.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		ground.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		A.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		B.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		C.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		F.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		xingkong.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		qiao.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		cat.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		D.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		H.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		E.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		G.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 32.0f);
		luzhang.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 128.0f);
		bicycle.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		modelShader.setMat4("model", model);
		// material properties
		modelShader.setFloat("material.shininess", 128.0f);
		lamp.Draw(modelShader);
		if (reset == 100)camera.Target = glm::vec3(-2.7, 30, -1.1);

		if(RoadSearchSwitch)
			for (int i = 0; i < Mymap.findRoad(destination, camera.Target) - 1; i++)
				drawSegment(Mymap.path[i], Mymap.path[i + 1], &lineSegmentShader, segmentID, view, projection);




		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		if (lighton && !day)
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_night);
		else
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_day);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default



							  //Update the frame
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		camera.ProcessKeyboard(JUMP, deltaTime);
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		RoadSearchSwitch = !RoadSearchSwitch;
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
		lighton = !lighton;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		day = !day;
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
		spoton = !spoton;
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(0.2*yoffset);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}

}

void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
		//walk = !walk;
		//std::cout << camera.Target.z << std::endl;
	}
	if (keys[GLFW_KEY_S]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		//walk = !walk;
	}
	if (keys[GLFW_KEY_A]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
		//walk = !walk;
	}
	if (keys[GLFW_KEY_D]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
		//walk = !walk;
	}
	if (keys[GLFW_KEY_R])
		camera.ReturnOriginView(HeroPosition + glm::vec3(0.0f, 5.0f, -5.0f));
	if (keys[GLFW_KEY_SPACE]) {
		//camera.ProcessKeyboard(JUMP, deltaTime);
	}
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


void drawSegment(glm::vec3 p1, glm::vec3 p2, Shader* shader, glm::mat4 view, glm::mat4 projection) {
	float vertices[] = {       
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f

	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind and set the buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glm::mat4 model = glm::mat4(1.0f);
	// apply transformations to the segment
	shader->use();
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	// Rendering
	glLineWidth(30.0f);
	glDrawArrays(GL_LINE, 0, 4);

	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void InitCrashCube(Wall *CB) {
	CB[0].pos = glm::vec3(-123, -5, -232.7);
	CB[0].length = 357;
	CB[0].width = 238.4;
	CB[0].height = 5;//地面1

	CB[1].pos = glm::vec3(-123, -5, 32.6);
	CB[1].length = 357;
	CB[1].width = 61.2;
	CB[1].height = 5;//地面2

	CB[2].pos = glm::vec3(-123, -5, -208);
	CB[2].length = 112.2;
	CB[2].width = 70;
	CB[2].height = 50;

	CB[3].pos = glm::vec3(-123, -5, -138);
	CB[3].length = 104.2;
	CB[3].width = 9;
	CB[3].height = 50; //楼1

	CB[4].pos = glm::vec3(-123, -5, -104);
	CB[4].length = 107;
	CB[4].width = 69.7;
	CB[4].height = 50;//楼2

	CB[5].pos = glm::vec3(15, -5, -208);
	CB[5].length = 112.2;
	CB[5].width = 70;
	CB[5].height = 50;

	CB[6].pos = glm::vec3(24, -5, -138);
	CB[6].length = 210;
	CB[6].width = 9.3;
	CB[6].height = 50;//楼3

	CB[7].pos = glm::vec3(20, -5, -104);
	CB[7].length = 214;
	CB[7].width = 70;
	CB[7].height = 50;//楼4

	CB[8].pos = glm::vec3(-4, -5, -88);
	CB[8].length = 13;
	CB[8].width = 79;
	CB[8].height = 50;//花坛（楼24之间）

	CB[9].pos = glm::vec3(12, -5, 1.4);
	CB[9].length = 21;
	CB[9].width = 37.1;
	CB[9].height = 7;

	CB[10].pos = glm::vec3(12, -5, 1.4);
	CB[10].length = 3;
	CB[10].width = 37.1;
	CB[10].height = 50;

	CB[11].pos = glm::vec3(30, -5, 1.4);
	CB[11].length = 3;
	CB[11].width = 37.1;
	CB[11].height = 50;//桥（靠近楼4）

	CB[12].pos = glm::vec3(-27, -5, 1.4);
	CB[12].length = 21;
	CB[12].width = 37.1;
	CB[12].height = 7;

	CB[13].pos = glm::vec3(-27, -5, 1.4);
	CB[13].length = 3;
	CB[13].width = 37.1;
	CB[13].height = 50;

	CB[14].pos = glm::vec3(-9, -5, 1.4);
	CB[14].length = 3;
	CB[14].width = 37.1;
	CB[14].height = 50;//桥（靠近楼2）

	CB[15].pos = glm::vec3(32.6, -5, -119);
	CB[15].length = 25.3;
	CB[15].width = 5.5;
	CB[15].height = 50;

	CB[16].pos = glm::vec3(82, -5, -119);
	CB[16].length = 25.3;
	CB[16].width = 5.5;
	CB[16].height = 50;

	CB[17].pos = glm::vec3(133.1, -5, -119);
	CB[17].length = 25.3;
	CB[17].width = 5.5;
	CB[17].height = 50;

	CB[18].pos = glm::vec3(181.6, -5, -119);
	CB[18].length = 25.3;
	CB[18].width = 5.5;
	CB[18].height = 50;

	CB[19].pos = glm::vec3(-55.1, -5, -119);
	CB[19].length = 25.3;
	CB[19].width = 5.5;
	CB[19].height = 50;

	CB[20].pos = glm::vec3(-114, -5, -119);
	CB[20].length = 25.3;
	CB[20].width = 5.5;
	CB[20].height = 50;//小花坛x6

	CB[21].pos = glm::vec3(-80, -5, -113.5);
	CB[21].length = 0.2;
	CB[21].width = 0.2;
	CB[21].height = 50;

	CB[22].pos = glm::vec3(-76, -5, -112.8);
	CB[22].length = 0.2;
	CB[22].width = 0.2;
	CB[22].height = 50;//柱子x2

	std::cout << "successfully init crash box." << std::endl;
}

unsigned int segmentVAO() {
	glm::vec3 p1 = glm::vec3(-0.5f, 0.0f, -0.1f);
	glm::vec3 p2 = glm::vec3(-0.5f, 0.0f, 0.1f);
	glm::vec3 p3 = glm::vec3(-0.5f, 0.2f, -0.1f);
	glm::vec3 p4 = glm::vec3(-0.5f, 0.2, 0.1f);

	glm::vec3 p5 = glm::vec3(0.5f, 0.0f, -0.1f);
	glm::vec3 p6 = glm::vec3(0.5f, 0.0f, 0.1f);
	glm::vec3 p7 = glm::vec3(0.5f, 0.2f, -0.1f);
	glm::vec3 p8 = glm::vec3(0.5f, 0.2, 0.1f);

	float vertices[] = {
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
		p3.x, p3.y, p3.z,
		p3.x, p3.y, p3.z,
		p2.x, p2.y, p2.z,
		p4.x, p4.y, p4.z,  // left

		p5.x, p5.y, p5.z,
		p6.x, p6.y, p6.z,
		p7.x, p7.y, p7.z,
		p7.x, p7.y, p7.z,
		p6.x, p6.y, p6.z,
		p8.x, p8.y, p8.z,  // right

		p4.x, p4.y, p4.z,
		p8.x, p8.y, p8.z,
		p3.x, p3.y, p3.z,
		p3.x, p3.y, p3.z,
		p8.x, p8.y, p8.z,
		p7.x, p7.y, p7.z,  // top

		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
		p6.x, p6.y, p6.z,
		p6.x, p6.y, p6.z,
		p2.x, p2.y, p2.z,
		p5.x, p5.y, p5.z, // down

		p1.x, p1.y, p1.z,
		p5.x, p5.y, p5.z,
		p3.x, p3.y, p3.z,
		p3.x, p3.y, p3.z,
		p5.x, p5.y, p5.z,
		p7.x, p7.y, p7.z, //back

		p2.x, p2.y, p2.z,
		p6.x, p6.y, p6.z,
		p4.x, p4.y, p4.z,
		p4.x, p4.y, p4.z,
		p6.x, p6.y, p6.z,
		p8.x, p8.y, p8.z // front
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind and set the buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	return VAO;
}

void drawSegment(glm::vec3 p1, glm::vec3 p2, Shader* shader, unsigned int VAO, glm::mat4 view, glm::mat4 projection) {

	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 dirP1toP2 = p2 - p1;

	glm::vec3 mediumPoint = 0.5f * dirP1toP2;
	mediumPoint += p1;
	model = translate(model, mediumPoint+glm::vec3(0,5,0));

	float angle = acos(dot(normalize(dirP1toP2), glm::vec3(1.0f, 0.0f, 0.0f)));
	if ((dirP1toP2.x > 0 && dirP1toP2.z > 0) or
		(dirP1toP2.x < 0 && dirP1toP2.z > 0)) {

		angle = -angle;
	}
	model = rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	float l = length(dirP1toP2);
	model = scale(model, glm::vec3(l, 1.0f, 1.0f));

	// apply transformations to the segment
	shader->use();
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	// Rendering
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// unbind
	glBindVertexArray(0);
}

void InitMap() {
	
	Type Bridge_A_Cat("0: Bridge_A_Cat", glm::vec3(-18, 0, 55));
	Type Bridge_B_Cat("1: Bridge_B_Cat", glm::vec3(22, 0, 55));
	Type Bridge_A("2: Bridge_A", glm::vec3(-18, 0, -3));
	Type Bridge_B("3: Bridge_B", glm::vec3(22, 0, -3));
	Type Crossing_A("4: Crossing_A", glm::vec3(14, 0, -28));
	Type Crossing_B("5: Crossing_B", glm::vec3(-10, 0, -28));
	Type Crossing_C("6: Crossing_C", glm::vec3(-10, 0, -102));
	Type Crossing_D("7: Crossing_D", glm::vec3(17, 0, -102));
	Type Crossing_E("8: Crossing_E", glm::vec3(-15, 0, -123));
	Type Crossing_F("9: Crossing_F", glm::vec3(22, 0, -123));
	Type Building_A("10: Building_A", glm::vec3(-82, 0, -106));
	Type Building_B("11: Building_B", glm::vec3(-71, 0, -127));
	Type Crossing_G("12: Crossing_G", glm::vec3(3, 0, -135));
	Type Building_D("13: Building_D", glm::vec3(124, 0, -107));
	Type Building_F("14: Building_F", glm::vec3(165, 0, -127));

	Mymap.insertVertex(Bridge_A_Cat);
	Mymap.insertVertex(Bridge_B_Cat);
	Mymap.insertVertex(Bridge_A);
	Mymap.insertVertex(Bridge_B);
	Mymap.insertVertex(Crossing_A);
	Mymap.insertVertex(Crossing_B);
	Mymap.insertVertex(Crossing_C);
	Mymap.insertVertex(Crossing_D);
	Mymap.insertVertex(Crossing_E);
	Mymap.insertVertex(Crossing_F);
	Mymap.insertVertex(Crossing_G);
	Mymap.insertVertex(Building_A);
	Mymap.insertVertex(Building_B);
	Mymap.insertVertex(Building_D);
	Mymap.insertVertex(Building_F);

	Mymap.insertEdge(Bridge_A_Cat, Bridge_B_Cat, 40);
	Mymap.insertEdge(Bridge_A, Bridge_A_Cat, 58);
	Mymap.insertEdge(Bridge_B_Cat, Bridge_B, 58);
	Mymap.insertEdge(Bridge_A, Crossing_B, 26);
	Mymap.insertEdge(Bridge_B, Crossing_A, 26);

	Mymap.insertEdge(Crossing_B, Crossing_C, 26);
	Mymap.insertEdge(Crossing_C, Building_A, 26);
	Mymap.insertEdge(Crossing_C, Crossing_D, 26);
	Mymap.insertEdge(Crossing_C, Crossing_E, 26);
	Mymap.insertEdge(Building_A, Building_B, 26);

	Mymap.insertEdge(Crossing_E, Building_B, 26);
	Mymap.insertEdge(Crossing_E, Crossing_F, 26);
	Mymap.insertEdge(Crossing_E, Crossing_G, 26);
	Mymap.insertEdge(Crossing_F, Crossing_G, 26);

	Mymap.insertEdge(Building_D, Building_F, 26);
	Mymap.insertEdge(Crossing_F, Crossing_D, 26);
	Mymap.insertEdge(Crossing_F, Building_F, 26);
	Mymap.insertEdge(Crossing_D, Building_D, 26);
	Mymap.insertEdge(Crossing_D, Crossing_A, 26);
}

