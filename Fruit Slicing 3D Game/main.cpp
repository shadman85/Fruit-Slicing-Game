//
//  main.cpp
//  3D Object Drawing
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "Sphere2.h"
#include "spotLight.h"
#include "cube.h"
#include "Cylinder.h"
#include "curve.h"
#include "stb_image.h"

#include <iostream>

//using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float run_speed = 0.004f;
float shoot_angle = 0.0f;
//initial values

float gun_x = 0.0f;
float gun_y = 0.5f;
float balloon_x = 1.5f;
float balloon_y = 1.2f;

bool fruit = true;
float fruit_y = 0.0f;
float inc = 1.0f;

//initial value for dynamite
//float dnx = 0.0f;
float dny = -2.5f;
bool dnm = true;

float theta = 0.0f;



// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f, 1.0f, -3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 1.0f, 0.0f,
    .09f, .09f, .09f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.50f,  1.50f,  0.0f),
    glm::vec3(1.5f,  1.5f,  10.0f),
    glm::vec3(-1.5f,  1.5f,  0.0f),
    glm::vec3(-1.5f,  1.5f,  10.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool shoot = false;
float bullet_z = -5.2f;
int score = 3;
bool touched = false;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float speed = 0.003;

float rx = 30.0f;
float ry = 0.0f;
float rz = 0.0f;
float tx=gun_x, ty=gun_y, tz=0.0f;
float  m = tan(glm::radians(rx));
bool an_inc = false;
bool an_dec = false;



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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    vector<float>apple_vertices = {
   -0.6400, 1.8500, 5.1000,
-0.9200, 1.8650, 5.1000,
-1.0050, 1.8700, 5.1000,
-1.1450, 1.7650, 5.1000,
-1.1850, 1.6550, 5.1000,
-1.1950, 1.5250, 5.1000,
-1.1900, 1.2700, 5.1000,
-1.1100, 1.0750, 5.1000,
-1.0550, 1.0050, 5.1000,
-0.9500, 0.8850, 5.1000,
-0.8900, 0.8350, 5.1000,
-0.7400, 0.8000, 5.1000,
-0.6300, 0.7900, 5.1000,
-0.5350, 0.7950, 5.1000,
-0.4750, 0.8050, 5.1000,
-0.4600, 0.8100, 5.1000,
    };

    vector<float>apple_vertices2 = {
        -0.0350, 1.7700, 5.1000,
-0.0700, 1.7550, 5.1000,
-0.1050, 1.7550, 5.1000,
-0.1650, 1.7550, 5.1000,
-0.1950, 1.7650, 5.1000,
-0.2200, 1.8050, 5.1000,
-0.2500, 1.8200, 5.1000,
-0.2850, 1.8200, 5.1000,
-0.3400, 1.8250, 5.1000,
-0.3800, 1.8200, 5.1000,
-0.4250, 1.7850, 5.1000,
-0.4500, 1.7300, 5.1000,
-0.4550, 1.6850, 5.1000,
-0.4550, 1.6300, 5.1000,
-0.4550, 1.6050, 5.1000,
-0.4450, 1.5750, 5.1000,
-0.4400, 1.5650, 5.1000,
-0.4350, 1.5500, 5.1000,
-0.4300, 1.5350, 5.1000,
-0.4300, 1.5150, 5.1000,
    };


    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();
    Sphere point = Sphere();
    point.setRadius(0.001f);
    float inc2 = 0.0f;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    string diffuseMapPath = "floor.png";
    string specularMapPath = "floor.png";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath22 = "game_over2.jpg";
    string specularMapPath22 = "game_over2.jpg";
    unsigned int diffMap22 = loadTexture(diffuseMapPath22.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap22 = loadTexture(specularMapPath22.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap22, specMap22, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathWall = "background.jpg";
    string specularMapPathWall ="background.jpg";
    unsigned int diffMapWall = loadTexture(diffuseMapPathWall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapWall = loadTexture(specularMapPathWall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wallcube = Cube(diffMapWall, specMapWall, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath0 = "Numbers/0.png";
    string specularMapPath0 = "Numbers/0.png";
    unsigned int diffMap0 = loadTexture(diffuseMapPath0.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap0 = loadTexture(specularMapPath0.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube0 = Cube(diffMap0, specMap0, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath1 = "Numbers/1.png";
    string specularMapPath1 = "Numbers/1.png";
    unsigned int diffMap1 = loadTexture(diffuseMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap1 = loadTexture(specularMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube1 = Cube(diffMap1, specMap1, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath2 = "Numbers/2.png";
    string specularMapPath2 = "Numbers/2.png";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube2= Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath3 = "Numbers/3.png";
    string specularMapPath3 = "Numbers/3.png";
    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube3 = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath4 = "Numbers/4.png";
    string specularMapPath4 = "Numbers/4.png";
    unsigned int diffMap4= loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap4 = loadTexture(specularMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube4 = Cube(diffMap4, specMap4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath5 = "Numbers/5.png";
    string specularMapPath5 = "Numbers/5.png";
    unsigned int diffMap5= loadTexture(diffuseMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap5 = loadTexture(specularMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube5 = Cube(diffMap5, specMap4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath6 = "Numbers/6.png";
    string specularMapPath6= "Numbers/6.png";
    unsigned int diffMap6 = loadTexture(diffuseMapPath6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap6= loadTexture(specularMapPath6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube6= Cube(diffMap6, specMap6, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath7 = "Numbers/7.png";
    string specularMapPath7 = "Numbers/7.png";
    unsigned int diffMap7 = loadTexture(diffuseMapPath7.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap7 = loadTexture(specularMapPath7.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube7= Cube(diffMap7, specMap7, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath8 = "Numbers/8.png";
    string specularMapPath8 = "Numbers/8.png";
    unsigned int diffMap8 = loadTexture(diffuseMapPath8.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap8 = loadTexture(specularMapPath8.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube8 = Cube(diffMap8, specMap8, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath9 = "Numbers/9.png";
    string specularMapPath9 = "Numbers/9.png";
    unsigned int diffMap9 = loadTexture(diffuseMapPath9.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap9 = loadTexture(specularMapPath9.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube9 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathlevel = "level.png";
    string specularMapPathlevel = "level.png";
    unsigned int diffMaplevel = loadTexture(diffuseMapPathlevel.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMaplevel = loadTexture(specularMapPathlevel.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube levelcube = Cube(diffMaplevel, specMaplevel, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPathwmo = "watermelon_out.jpg";
    string specularMapPathwmo = "watermelon_out.jpg";
    unsigned int diffMapwmo = loadTexture(diffuseMapPathwmo.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapwmo = loadTexture(specularMapPathwmo.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 waterOut = Sphere2(0.1f, 144, 72, glm::vec3(0.4f, 0.259f, 0.125f), glm::vec3(0.4f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMapwmo, specMapwmo, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathwmi = "watermelon_in.png";
    string specularMapPathwmi = "watermelon_in.png";
    unsigned int diffMapwmi = loadTexture(diffuseMapPathwmi.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapwmi = loadTexture(specularMapPathwmi.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 waterIn = Sphere2(0.09f, 144, 72, glm::vec3(0.4f, 0.259f, 0.125f), glm::vec3(0.4f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMapwmi, specMapwmi, 0.0f, 0.0f, 1.0f, 1.0f);




    Cylinder cylinder(.08, .08, .35, 32, 20, "blade.jpg");

    Cylinder cylinder2(.001, .01, .35, 32, 20, "blade.jpg");

    Cylinder cylinder1(.08, .08, .35, 32, 20, "black_dyn.jpg");


    string diffApple = "apple.png";
    string specApple= "apple.png";
    unsigned int diffMapApple = loadTexture(diffApple.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapApple = loadTexture(specApple.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve apple(apple_vertices, diffMapApple, specMapApple, 1.0f);

    Curve apple2(apple_vertices2, diffMapApple, specMapApple, 1.0f);


    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------

    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    float randomValue = dis(gen);
    float dnx = dis(gen);


    int count = 1;
    int num = 0;
    float angle = 90.0f;
    float extra = 0.0f;
    float extra1 = 0.0f;
    float draw = true;
    bool show_bdn = false;
    int level = 1;


    while (!glfwWindowShouldClose(window))
    {

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Position = glm::vec3(0.0f, 1.0f, -3.0f);



        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();

        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);



        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        func(cubeVAO, lightingShader, model);

     
      /*  cout << gun_x << " " << gun_y << endl;
        cout << tx << " -- " << ty << endl;*/

        // firing cubes
        if (shoot)
        {
            if (rx < 90) {
                ty = m * tx;
                tx += 0.01f;
            }
            if (rx > 90 and rx < 180) {
                ty = m * tx;
                tx -= 0.01f;
            }
            if (tx * tx + ty * ty > 5.0f)
            {
                shoot = false;


            }
            
            // bullet cube
         /*   glm::mat4 bullet = glm::mat4(1.0f);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-tx, ty, tz));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-rx), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.07f, 0.05f));
            bullet = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, bullet, 1.0, 0.0, 0.0);*/

            glm::mat4 bullet = glm::mat4(1.0f);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-tx-.05f, ty-0.05f, 0.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, -1.2f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-rx), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.03f, 0.05f));
            bullet = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, bullet, 1.0, 0.0, 0.0);

            // cout << "bulletfront" << endl;
            glm::mat4 bulletFront = glm::mat4(1.0f);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-tx-0.05f , ty ,0.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-rx-90), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 0.5f, 10.0f));
            bulletFront = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            cylinder2.Draw(lightingShaderWithTexture, bulletFront);

        }
      

      

        speed = score * 0.00025f;
        fruit_y += speed;
        dny += speed;
        if (score == 3)
        {
            draw = true;
        }

        if (2.5f - fruit_y <= 0.0f)
        {
            fruit_y = 0.0f;
            randomValue = dis(gen);
            angle = 90.0f;
            extra = 0.0f;
            theta = 0.0f;
            if (count != num && draw)
            {
               score--;
            }
            count++;
            num = count - 1;
        }

        

        // collision condition and score add
        //cout<<"randomValue = "<<randomValue<<endl;
        if (shoot)
        {

            float yy = 2.5f - fruit_y;
            float dist = sqrt((tx + randomValue) * (tx + randomValue) + (ty - yy) * (ty - yy));
            cout << "dist = " << dist << endl;
            if (dist < 0.2f)
            {
                touched = true;
                angle = 110.0f;
                extra = 0.05f;
                theta = 20.0f;
                if (num != count)
                {
                    if (score < 99)
                    {
                        score++;
                    }

                    num = count;


                }
            }
        }
        
        
        // condition for dynamite
        //if (2.5f - dny <= 0.0f)
        //{
        //    dny = 0.0f;
        //    dnx = dis(gen);
        //    angle = 90.0f;
        //    extra1 = 0.0f;
        //   /* if (count != num && draw)
        //    {
        //        score--;
        //    }*/
        //    count++;
        //    num = count - 1;
        //}

       /* if (shoot)
        {
            if (randomValue + 0.1f >= gun_x && randomValue - 0.1f <= gun_x)
            {
                float yy = 2.5f - fruit_y;
                if (gun_y + 0.5f >= yy - 0.1f && yy + 0.1f >= gun_y)
                {
                    touched = true;
                    angle = 110.0f;
                    extra = 0.1;
                    if (num != count)
                    {
                        if (score < 99)
                        {
                            score++;
                        }

                        num = count;


                    }

                }
            }*/
        
           
        
       
       
     



        /*float xx = 0.03f;
        for (int i = 0; i < score; i++)
        {
            glm::mat4 modelForSphere = glm::mat4(1.0f);
            translateMatrix = glm::translate(model, glm::vec3(-0.5f - xx, 1.5f, -1.2f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            modelForSphere = modelForSphere * translateMatrix * rotateZMatrix * scaleMatrix;
            point.drawSphere(lightingShader, modelForSphere);

            glm::mat4 modelForSphere2 = glm::mat4(1.0f);
            translateMatrix = glm::translate(model, glm::vec3(-0.5f - xx, 1.5f, -1.2f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            modelForSphere2 = modelForSphere2 * translateMatrix * rotateZMatrix * scaleMatrix;
            point.drawSphere(lightingShader, modelForSphere2);
            xx += 0.1f;
        }*/


        if (score >=25 && fruit)
        {
        //    // fruit-1
        // /*   glm::mat4 modelForSphere = glm::mat4(1.0f);
        //    translateMatrix = glm::translate(model, glm::vec3(randomValue - extra, 2.5f - fruit_y, -1.2f));
        //    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        //    modelForSphere = modelForSphere * translateMatrix * rotateZMatrix;
        //    sphere.drawSphere(lightingShader, modelForSphere);

        //    glm::mat4 modelForSphere2 = glm::mat4(1.0f);
        //    translateMatrix = glm::translate(model, glm::vec3(randomValue + extra, 2.5f - fruit_y, -1.2f));
        //    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
        //    modelForSphere2 = modelForSphere2 * translateMatrix * rotateZMatrix;
        //    sphere.drawSphere(lightingShader, modelForSphere2);*/

        //    //randomValue = 0.0f;

            glm::mat4 modelForSphere = glm::mat4(1.0f);
            translateMatrix = glm::translate(model, glm::vec3(randomValue + extra, 2.5f - fruit_y, -1.2f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            modelForSphere = modelForSphere * translateMatrix * rotateXMatrix * rotateYMatrix;
            waterOut.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere);
            waterIn.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere);

            glm::mat4 modelForSphere2 = glm::mat4(1.0f);
            translateMatrix = glm::translate(model, glm::vec3(randomValue - extra, 2.5f - fruit_y, -1.2f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            modelForSphere2 = modelForSphere2 * translateMatrix * rotateXMatrix * rotateYMatrix;
            waterOut.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere2);
            waterIn.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere2);
        }



        if (score > 0 && score<25 && fruit)
        {

           // translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.87f, -1.5f));
            translateMatrix = glm::translate(model, glm::vec3(randomValue + extra, 2.5f - fruit_y, -1.2f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-theta), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.35f));
            glm::mat4 curve_model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            apple2.draw(lightingShaderWithTexture, curve_model, glm::vec3(1.0f, 1.0f, 1.0f));

            //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 1.5f, -1.5f));
            translateMatrix = glm::translate(model, glm::vec3(randomValue - extra, 2.5f - fruit_y+0.94f, -1.2f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(theta), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.35f));
            glm::mat4 curve_model2 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            apple2.draw(lightingShaderWithTexture, curve_model2, glm::vec3(1.0f, 1.0f, 1.0f));


        }

        

       

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights


        //glBindVertexArray(lightCubeVAO);
        //for (unsigned int i = 0; i < 4; i++)
        //{
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, pointLightPositions[i]);
        //    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //    ourShader.setMat4("model", model);
        //    ourShader.setVec3("color", glm::vec3(cr, cg, cb));
        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //    //glDrawArrays(GL_TRIANGLES, 0, 36);
        //}


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        // wall with texture
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 0.0f, 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 5.0f, 0.1f));
        glm::mat4 wallmodel = translateMatrix * scaleMatrix;;
        wallcube.drawCubeWithTexture(lightingShaderWithTexture, wallmodel);


        //grass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 0.1f, -5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 0.2f, 10.0f));
        glm::mat4 modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);




        if (score == 0)
        {
            draw = false;
            //game over
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -0.7f, 1.9f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 5.0f, 0.1f));
            glm::mat4 modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            cube2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);
        }
        //if (score > 0 && dnm)
        //{
        //    translateMatrix = glm::translate(identityMatrix, glm::vec3(dnx,2.5f- dny - extra1, 0.0f));
        //    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        //    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        //    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.5f, 1.0f));
        //    glm::mat4 cmodel = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        //    cylinder1.Draw(lightingShaderWithTexture, cmodel);

          /*  translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 cmodel = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            cylinder.Draw(lightingShaderWithTexture, cmodel);*/


        //    //score -= 1;
        //   // score = max(0, score);

        //}

        // gun base 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.05f+0.05f, gun_y - 0.16f , -1.2f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.5f,1.0f));
        glm::mat4 cmodel = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        cylinder.Draw(lightingShaderWithTexture, cmodel);



        // curve apple
       /* translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f,1.0f , -1.5f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.3f));
        glm::mat4 curve_model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        apple.draw(lightingShaderWithTexture, curve_model, glm::vec3(1.0f, 1.0f, 1.0f));*/

    /*    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.87f, -1.5f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.6f));
        glm::mat4 curve_model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        apple2.draw(lightingShaderWithTexture, curve_model, glm::vec3(1.0f, 1.0f, 1.0f));

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 1.5f, -1.5f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.6f));
        glm::mat4 curve_model2 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        apple2.draw(lightingShaderWithTexture, curve_model2, glm::vec3(1.0f, 1.0f, 1.0f));
        */



         
       









            
        
        










        //display score and levels

        float rotateAngle_Xx = -10;
        float rotateAngle_Yy = 70;
        float rotateAngle_Zz = -30;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.50f, 1.8f, 0.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Xx), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Yy), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Zz), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02f, 0.2f, .2f));
        glm::mat4 digitCube1 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.29f, 1.8f, 0.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Xx), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Yy), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Zz), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02f, 0.2f, .2f));
        glm::mat4 digitCube2 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;


        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8f, 2.0f, 0.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02f, 0.15f, 0.2f));
        glm::mat4 levelCube = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        /*float rotateAngle_Xx = -10;
        float rotateAngle_Yy = 70;
        float rotateAngle_Zz = -30;*/
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f,2.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02f, 0.15f, 0.5f));
        glm::mat4 levelTextCube = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;




        //cube1.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);

        int d1, d2;
        d1 = score % 10;
        int n = score / 10;
        d2 = n % 10;
        
        // 1st digit
        if (d1 == 0)
        {
            dcube0.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 1)
        {
            dcube1.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 2)
        {
            dcube2.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 3)
        {
            dcube3.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 4)
        {
            dcube4.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 5)
        {
            dcube5.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 6)
        {
            dcube6.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 7)
        {
            dcube7.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 8)
        {
            dcube8.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }
        else  if (d1 == 9)
        {
            dcube9.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        }


        //2nd digit

        if (d2 == 0)
        {
            dcube0.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2 == 1)
        {
            dcube1.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2 == 2)
        {
            dcube2.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2 == 3)
        {
            dcube3.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2 == 4)
        {
            dcube4.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2 == 5)
        {
            dcube5.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2 == 6)
        {
            dcube6.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2== 7)
        {
            dcube7.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2 == 8)
        {
            dcube8.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }
        else  if (d2== 9)
        {
            dcube9.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
        }


        // level show 

        levelcube.drawCubeWithTexture(lightingShaderWithTexture, levelTextCube);

        if (score < 10)

        {
            level = 1;
        }


            
        if (score >= 10)
        {
            level = 2;
  

        }
        if (score >= 25)
        {
            level = 3;


        }
        
           

            if (level == 1)
            {
                dcube1.drawCubeWithTexture(lightingShaderWithTexture, levelCube);
            }
            if (level == 2)
            {
                dcube2.drawCubeWithTexture(lightingShaderWithTexture, levelCube);
            }
            if (level == 3)
            {
                dcube3.drawCubeWithTexture(lightingShaderWithTexture, levelCube);
            }
        
        
       


        



        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.05f, 0.1f, grass2));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        //glm::mat4 modelMatrixForContainer4 = translateMatrix * scaleMatrix;
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float baseHeight = 0.1;
    float width = 5;
    float length = 10;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.1, 0.5, 0.3);


    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;


   



   
  

    //gun that is rotating 
    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.03f, gun_y -0.16f, -1.2f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.4f, 0.05f));
    //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(an - 90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = alTogether * translateMatrix * rotateXMatrix * scaleMatrix;
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-rx+90), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * translateMatrix * rotateZMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 0.5);

    //green base of gun
  /*  translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.05f , gun_y - 0.16f-.13f , -1.5f ));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.2f, 0.1f));
    glm::mat4 cmodel = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, cmodel, 0.1, 0.5, 0.3);*/


    //shoot





}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gun_y += 0.003f;
        

        camera.ProcessKeyboard(FORWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gun_y -= 0.003f;
       
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gun_x += 0.003f;
        
        camera.ProcessKeyboard(LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gun_x -= 0.003f;
       
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
       /* if (!shoot)
            shoot = true;*/
        if (!shoot)
        {
            shoot = true;
            tx = gun_x - 0.03f;
            ty = gun_y - 0.16f;
            m = tan(glm::radians(rx));


        }
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        if (score == 0)
            score = 3;
        //count = 1;
        //num = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        /*if (!an_inc)
            an_inc = true;*/
       
            if (rx < 150.0f) rx += 0.5;
            if (rx == 90) rx += 0.5;



        
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
       /* if (!an_dec)
            an_dec = true;*/
            //    if (rx > 0.0f) rx -= 10;


              if (rx > 30.0f) rx -= 0.5;
            if (rx == 90) rx -= 0.5;



    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
       

        score = 30;


    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {


        score = 20;


    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {


        score = 5;


    }






}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}