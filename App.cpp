#include <iostream>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "App.h"
#include "FrameCounter.h"
#include "DebugOutputManager.h"
#include "Window.h"
#include "OBJLoader.h"
#include "Mesh.h"
#include "Shader.h"

App::App()
{
    // default constructor
    // nothing to do here (so far...)
    std::cout << "New App constructed\n";
    window = new Window(800, 600, "OpenGL Window");;
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

    auto vertexShaderPath = std::filesystem::path("./assets/shaders/basic.vert");
    auto fragmentShaderPath = std::filesystem::path("./assets/shaders/basic.frag");
    auto shader = Shader(vertexShaderPath, fragmentShaderPath);
    auto mesh = test.getMesh();

    std::cout << shader.ID << std::endl;

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


        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setUniform("transform", trans);

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, -1.5f, -12.5f));
        shader.setUniform("view", view);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
        shader.setUniform("projection", projection);

        mesh.draw(shader);

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