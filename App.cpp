#include <iostream>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <opencv2\opencv.hpp>

#include "App.h"
#include "Window.h"
#include "Camera.h"
#include "OBJLoader.h"
#include "Mesh.h"
#include "Shader.h"

#include "FrameCounter.h"
#include "DebugOutputManager.h"

#include "Projectile.h"
#include "texture.h"


// Deklarace globální instance Projectile
Projectile projectile(glm::vec3(0.0f)); // Poèáteèní pozice støely

float speedOfProjectiles = 0.05f;

Texture texture;

//const char* texturePath = "./assets/textures/box.png"; // cesta k obrázku textury
std::string texturePath = "./assets/textures/box.png"; // cesta k obrázku textury
//auto myPath = std::filesystem::path("./assets/textures/box.png");
//GLuint textureID = textureInit(texturePath); // naèíst texturu a získat její ID
//GLuint textureID = loadTexture(texturePath); // naèíst texturu a získat její ID
auto textureID = 0;

// Funkce pro obsluhu kliknutí myší
void onMouseClick(GLFWwindow* window, int button, int action, int mods) {

    glm::vec3 cameraPosition = App::camera.getPosition();
    std::cout << "Pozice kamery: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;

    // Volání metody onKeyboardEvent tøídy Projectile
    projectile.onKeyboardEvent(window, cameraPosition, button, action, mods);
}

void drawProjetiles(Shader shader) {
    // Projektily jsou uloženy v poli projectiles
    std::queue<Projectile> temporaryQueue = projectile.getAllProjectiles();
    std::cout << "_position: (" << projectile.position.x << ", " << projectile.position.y << ", " << projectile.position.z << ")" << std::endl;

    //glm::vec3 movement(0.5f, 0.0f, 0.0f);
    // Získat smìr pohledu kamery
    glm::vec3 cameraFront = App::camera.getFront();
    // Normalizovat smìr pohledu kamery
    cameraFront = glm::normalize(cameraFront);
    // Vypoèítat vektor pohybu
    glm::vec3 movement = speedOfProjectiles * cameraFront;

    projectile.addMovementToAllProjectiles(movement);

    while (!temporaryQueue.empty()) {
        Projectile currentProjectile = temporaryQueue.front(); // Získat aktuální projektil z fronty
        temporaryQueue.pop(); // Odstranit aktuální projektil z fronty

        // Upravit pozici projektilu ve shaderu
        shader.setUniform("model", glm::translate(glm::mat4(1.0f), currentProjectile.position));

        // Vykreslit projektil na jeho pozici
        currentProjectile.drawProjectile(currentProjectile.position);
    }
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

    // important -----
    InitAssets();

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

     // Registrace funkce pro obsluhu kliknutí myší
    glfwSetMouseButtonCallback(window->getWindow(), onMouseClick);

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    std::cout << shader.ID << std::endl;
    static int prevX = -1;
    static int prevY = -1;
    bool isResettingCursor = false;
    int width, height, centerX, centerY;

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

        glm::mat4 mx_view = camera.getViewMatrix();

        // Set Model Matrix
        UpdateModels();

        // Activate shader, set uniform vars
        shader.activate();
        //my_shader.SetUniform("uMx_model", mx_model); // Object local coor space -> World space
        shader.setUniform("uMx_view", mx_view); // World space -> Camera space
        shader.setUniform("uMx_projection", mx_projection); // Camera space -> Screen

        ///*
        shader.setUniform("ambient_material", rgb_white);
        shader.setUniform("diffuse_material", rgb_white);
        shader.setUniform("specular_material", rgb_white);
        shader.setUniform("specular_shinines", 5.0f);
        shader.setUniform("light_position", light_position);


        // Draw the scene
            // - Draw opaque objects
        for (auto& [key, value] : scene_opaque) {
            value.Draw(shader);
        }
        // - Draw transparent objects
        glEnable(GL_BLEND);         // enable blending
        glDisable(GL_CULL_FACE);    // no polygon removal
        glDepthMask(GL_FALSE);      // set Z to read-only
        // TODO: sort by distance from camera, from far to near
        for (auto& [key, value] : scene_transparent) {
            value.Draw(shader);
        }
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);

        drawProjetiles(shader);

        // === End of frame ===
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
