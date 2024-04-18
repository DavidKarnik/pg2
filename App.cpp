#include <iostream>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "App.h"
#include "Window.h"
#include "Camera.h"
#include "OBJLoader.h"
#include "Mesh.h"
#include "Shader.h"

#include "FrameCounter.h"
#include "DebugOutputManager.h"

#include "Projectile.h"

// Deklarace globální instance Projectile
Projectile projectile(glm::vec3(0.0f)); // Poèáteèní pozice støely

auto camera = Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };

// Funkce pro obsluhu kliknutí myší
void onMouseClick(GLFWwindow* window, int button, int action, int mods) {
    // Získání pozice kurzoru myši
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glm::vec3 cursorPosition(xPos, yPos, 0.0f);

    glm::vec3 cameraPosition = camera.getPosition();
    std::cout << "Pozice kamery: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;

    // Volání metody onKeyboardEvent tøídy Projectile
    projectile.onKeyboardEvent(window, cameraPosition, button, action, mods);
}


App::App()
{
    // default constructor
    // nothing to do here (so far...)
    std::cout << "New App constructed\n";
    window = new Window(800, 600, "3D Game");
}

bool App::init()
{
    // init glew
    // http://glew.sourceforge.net/basic.html
    glewInit();
    wglewInit();

    return true;
}

void App::report(void)
{
    GLint extensionCount;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);

    std::cout << "OpenGL Information:" << std::endl;
    std::cout << "Vendor: " << "\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << "\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << "\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "Extensions: " << "\t" << extensionCount << std::endl;


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
}

int App::run()
{
    FrameCounter fps;
    DebugOutputManager debug;

    std::cout << "Debug Output: \t" << (debug.isAvailable ? "yes" : "no") << std::endl;

    OBJLoader test{ "./assets/obj/bunny_tri_vnt.obj" };

    //auto camera = Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };
    auto mesh = test.getMesh();


    // Globální promìnná pro projektil
    //Projectile projectile(glm::vec3(0.0f)); // Poèáteèní pozice støely
    //glfwSetMouseButtonCallback(window, onMouseClick);
    //projectile.onKeyboardEvent(window->getWindow(), projectile, camera.getPosition(), );
     // Registrace funkce pro obsluhu kliknutí myší
    glfwSetMouseButtonCallback(window->getWindow(), onMouseClick);

    auto vertexShaderPath = std::filesystem::path("./assets/shaders/basic.vert");
    auto fragmentShaderPath = std::filesystem::path("./assets/shaders/newLight.frag");
    auto shader = Shader(vertexShaderPath, fragmentShaderPath);

    shader.setUniform("projection", camera.getProjectionMatrix());
    //glfwSetCursorPosCallback(window->getWindow(), camera.onMouseEvent);

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    std::cout << shader.ID << std::endl;
    static int prevX = -1;
    static int prevY = -1;
    bool isResettingCursor = false;
    int width, height, centerX, centerY;
    //glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 objectCenter = glm::vec3(0.0f, 0.0f, 0.0f); // Pøedpokládáme, že støed objektu je na poèátku souøadnic
    glm::vec3 lightPos = objectCenter + glm::vec3(0.0f, 3.0f, 0.0f); // Svìtlo bude umístìno 3 jednotky nad støedem objektu
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
    while (!glfwWindowShouldClose(window->getWindow()))
    {
        // If a second has passed.
        if (fps.hasSecondPassed())
        {
            // Display the frame count here any way you want.
            std::cout << "FPS: \t" << fps.getNumberOfFrames() << std::endl;
            fps.setNumberOfFrames(0);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSetMouseButtonCallback(window->getWindow(), onMouseClick);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /*lightPos.x = 5.0f;
        lightPos.y = 5.0f;*/
        /*lightPos.x = sin(glfwGetTime()) * 2.0f;
        lightPos.y = cos(glfwGetTime()) * 2.0f;*/

        //shader.setUniform("lightPos", lightPos);
        //shader.setUniform("viewPos", camera.getPosition());
        shader.setUniform("view", camera.getViewMatrix());
        shader.setUniform("projection", camera.getProjectionMatrix());

        POINT p;
        if (GetCursorPos(&p)) {
            // Získat rozmìry okna
            glfwGetWindowSize(window->getWindow(), &width, &height);
            centerX = width / 2;
            centerY = height / 2;
            if (isResettingCursor) {
                isResettingCursor = false;
                prevX = centerX;
                prevY = centerY;
            }
            float deltaX = static_cast<float>(p.x - prevX);
            float deltaY = static_cast<float>(p.y - prevY);
            bool isCursorOutOfCenter = p.x != 400 || p.y != 300;

            // Aktualizovat pøedchozí pozici pro další iteraci
            prevX = p.x;
            prevY = p.y;

            // Pokud došlo ke zmìnì pozice, volat onMouseEvent s relativní zmìnou
            if ((deltaX != 0 || deltaY != 0) && isCursorOutOfCenter) {
                camera.onMouseEvent(deltaX, deltaY, true);
                isResettingCursor = true;
                glfwSetCursorPos(window->getWindow(), centerX, centerY);
            }
        }




        camera.onKeyboardEvent(window->getWindow(), deltaTime); // process keys etc
        // Inicializace pozice svìtla
        glm::vec3 lightPos(1.0f, 1.0f, 1.0f);
        // Nastavení uniformní promìnné pro pozici svìtla ve vertex shaderu
        int lightPosLoc = glGetUniformLocation(shader.ID, "lightPos");
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        //glm::mat4 trans = glm::mat4(1.0f);
        ////trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        //shader.setUniform("transform", trans);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(20.0f, 20.0f, 20.0f)); // Nastavení translace (pozice v prostoru)
        shader.setUniform("transform", trans); // Nastavení uniformní promìnné pro transformaci ve vertex shaderu


        shader.setUniform("view", camera.getViewMatrix());

        //glm::mat4 projection = glm::mat4(1.0f);
        //projection = glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
        shader.setUniform("projection", camera.getProjectionMatrix());
        /*shader.setUniform("lightPos", lightPos);
        shader.setUniform("lightColor", lightColor);
        shader.setUniform("objectColor", objectColor);*/
        //shader.setUniform("viewPos", camera.getPosition());
        shader.setUniform("view", camera.getViewMatrix());
        shader.setUniform("projection", camera.getProjectionMatrix());
        shader.setUniform("transform", trans);
        mesh.draw(shader);


        // Vykreslit kostku
        //drawCube(10.0f, 0.0f, 0.0f, 0.0f);
        //projectile.drawCube3();
        projectile.drawAllProjectiles(5.0f);

        // Swap front and back buffers
        glfwSwapBuffers(window->getWindow());

        // Poll for and process events
        glfwPollEvents();

        
    }

    std::cout << std::endl;

    return 0;
}

App::~App()
{
    // clean-up
    std::cout << "Bye...\n";
}
