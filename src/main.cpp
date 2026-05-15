#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <common.h>

#include <learnopengl/shader.h>
#include <learnopengl/model.h>

#include <game/player.h>
#include <game/obstacle.h>
#include <skeletal/animation.h>
#include <skeletal/animator.h>

#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Player* player = nullptr;
Obstacle* obstacle = nullptr;

Animation* runAnimation = nullptr;
Animation* jumpAnimation = nullptr;
Animator* runAnimator = nullptr;
Animator* jumpAnimator = nullptr;
Animator* currentAnimator = nullptr;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool isGameOver = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadCubemap(vector<std::string> faces);
unsigned int loadTexture(char const* path);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLproject1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    float planeVertices[] = {
         2.0f, -0.5f,  10.0f,    0.0f, 1.0f, 0.0f,    1.0f,   0.0f,
        -2.0f, -0.5f,  10.0f,    0.0f, 1.0f, 0.0f,    0.0f,   0.0f,
        -2.0f, -0.5f, -100.0f,   0.0f, 1.0f, 0.0f,    0.0f,  30.0f,

         2.0f, -0.5f,  10.0f,    0.0f, 1.0f, 0.0f,    1.0f,   0.0f,
        -2.0f, -0.5f, -100.0f,   0.0f, 1.0f, 0.0f,    0.0f,  30.0f,
         2.0f, -0.5f, -100.0f,   0.0f, 1.0f, 0.0f,    1.0f,  30.0f,

        -2.0f,  0.0f,  10.0f,    0.0f, 1.0f, 0.0f,    0.05f,  0.0f,
        -2.2f,  0.0f,  10.0f,    0.0f, 1.0f, 0.0f,    0.00f,  0.0f,
        -2.2f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,    0.00f, 30.0f,

        -2.0f,  0.0f,  10.0f,    0.0f, 1.0f, 0.0f,    0.05f,  0.0f,
        -2.2f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,    0.00f, 30.0f,
        -2.0f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,    0.05f, 30.0f,

        -2.0f,  0.0f,  10.0f,    1.0f, 0.0f, 0.0f,    0.125f, 0.0f,
        -2.0f, -0.5f,  10.0f,    1.0f, 0.0f, 0.0f,    0.000f, 0.0f,
        -2.0f, -0.5f, -100.0f,   1.0f, 0.0f, 0.0f,    0.000f, 30.0f,

        -2.0f,  0.0f,  10.0f,    1.0f, 0.0f, 0.0f,    0.125f, 0.0f,
        -2.0f, -0.5f, -100.0f,   1.0f, 0.0f, 0.0f,    0.000f, 30.0f,
        -2.0f,  0.0f, -100.0f,   1.0f, 0.0f, 0.0f,    0.125f, 30.0f,

         2.2f,  0.0f,  10.0f,    0.0f, 1.0f, 0.0f,    0.05f,  0.0f,
         2.0f,  0.0f,  10.0f,    0.0f, 1.0f, 0.0f,    0.00f,  0.0f,
         2.0f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,    0.00f, 30.0f,

         2.2f,  0.0f,  10.0f,    0.0f, 1.0f, 0.0f,    0.05f,  0.0f,
         2.0f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,    0.00f, 30.0f,
         2.2f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,    0.05f, 30.0f,

         2.0f,  0.0f,  10.0f,   -1.0f, 0.0f, 0.0f,    0.125f, 0.0f,
         2.0f, -0.5f,  10.0f,   -1.0f, 0.0f, 0.0f,    0.000f, 0.0f,
         2.0f, -0.5f, -100.0f,  -1.0f, 0.0f, 0.0f,    0.000f, 30.0f,

         2.0f,  0.0f,  10.0f,   -1.0f, 0.0f, 0.0f,    0.125f, 0.0f,
         2.0f, -0.5f, -100.0f,  -1.0f, 0.0f, 0.0f,    0.000f, 30.0f,
         2.0f,  0.0f, -100.0f,  -1.0f, 0.0f, 0.0f,    0.125f, 30.0f,
    };

    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    float skyboxVertices[] = {
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

    unsigned int floorTexture = loadTexture("resources/textures/stone.jpg");

    vector<std::string> faces
    {
        "resources/textures/skybox/right.png",
        "resources/textures/skybox/left.png",
        "resources/textures/skybox/top.png",
        "resources/textures/skybox/bottom.png",
        "resources/textures/skybox/front.png",
        "resources/textures/skybox/back.png"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    Shader animShader("shader/anim.vs", "shader/anim.fs");
    Shader skyboxShader("shader/skybox.vs", "shader/skybox.fs");
    Shader staticShader("shader/static.vs", "shader/static.fs");

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    Model playerModel("resources/character/mixx.fbx");
    Model obstacleModel("resources/obstacle/wood-structure-part.obj");

    runAnimation = new Animation("resources/animation/running.fbx", &playerModel);
    jumpAnimation = new Animation("resources/animation/jumping.fbx", &playerModel);
    runAnimator = new Animator(runAnimation);
    jumpAnimator = new Animator(jumpAnimation);
    currentAnimator = runAnimator;

    player = new Player(&playerModel, glm::vec3(0.0f, 0.0f, 0.0f));
    obstacle = new Obstacle(&obstacleModel, glm::vec3(0.0f, 0.0f, -15.0f), 8.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    lastFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (!isGameOver) {
            player->update(deltaTime);
            obstacle->update(deltaTime);

            if (!player->isJumping)
                currentAnimator = runAnimator;

            if (currentAnimator == jumpAnimator)
                currentAnimator->UpdateAnimation(deltaTime, false);
            else
                currentAnimator->UpdateAnimation(deltaTime, true);

            if (obstacle->checkCollision(*player)) {
                isGameOver = true;
                std::cout << "게임오버 - 스페이스바로 재시작\n";
            }
        }
        glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::vec3 lightDir = glm::vec3(-0.4f, -1.0f, -0.2f);
        glm::vec3 lightColor = glm::vec3(1.0f);

        staticShader.use();
        staticShader.setMat4("projection", projection);
        staticShader.setMat4("view", view);
        staticShader.setVec3("lightDir", lightDir);
        staticShader.setVec3("viewPos", cameraPos);
        staticShader.setVec3("lightColor", lightColor);
        staticShader.setFloat("time", glfwGetTime());

        if (isGameOver)
            staticShader.setFloat("obstacleSpeed", 0.0f);
        else
            staticShader.setFloat("obstacleSpeed", 8.0f * (30.0f / 110.0f) * 1.25f);

        glm::mat4 model = glm::mat4(1.0f);
        staticShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        staticShader.setInt("texture_diffuse1", 0.0f);

        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 30);

        staticShader.setFloat("obstacleSpeed", 0);
        obstacle->draw(staticShader);

        animShader.use();
        animShader.setMat4("projection", projection);
        animShader.setMat4("view", view);
        animShader.setVec3("lightDir", lightDir);
        animShader.setVec3("viewPos", cameraPos);
        animShader.setVec3("lightColor", lightColor);

        auto transforms = currentAnimator->GetFinalBoneMatrices();
        auto loc = glGetUniformLocation(animShader.ID, "finalBoneMatrices");
        glUniformMatrix4fv(loc, transforms.size(), GL_FALSE, glm::value_ptr(transforms[0]));

        player->draw(animShader);

        glDepthFunc(GL_LEQUAL);

        skyboxShader.use();

        glm::mat4 skyView = glm::mat4(glm::mat3(view));
        skyboxShader.setMat4("view", skyView);
        skyboxShader.setMat4("projection", projection);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete player;
    delete obstacle;
    delete runAnimator;
    delete jumpAnimator;
    delete runAnimation;
    delete jumpAnimation;

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (isGameOver) {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            player->position = glm::vec3(0.0f, 0.0f, 0.0f);
            player->isJumping = false;
            player->yVelocity = 0;

            obstacle->position = glm::vec3(0.0f, 0.0f, -15.0f);

            jumpAnimator->Reset();
            currentAnimator = runAnimator;
            isGameOver = false;
        }
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!player->isJumping) {
            player->jump();
            jumpAnimator->Reset();
            currentAnimator = jumpAnimator;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
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

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 3);
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