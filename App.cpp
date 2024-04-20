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


// Deklarace glob�ln� instance Projectile
Projectile projectile(glm::vec3(0.0f)); // Po��te�n� pozice st�ely

auto camera = Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };

Texture texture;

//const char* texturePath = "./assets/textures/box.png"; // cesta k obr�zku textury
std::string texturePath = "./assets/textures/box.png"; // cesta k obr�zku textury
//auto myPath = std::filesystem::path("./assets/textures/box.png");
//GLuint textureID = textureInit(texturePath); // na��st texturu a z�skat jej� ID
//GLuint textureID = loadTexture(texturePath); // na��st texturu a z�skat jej� ID
auto textureID = 0;

// Funkce pro obsluhu kliknut� my��
void onMouseClick(GLFWwindow* window, int button, int action, int mods) {

    glm::vec3 cameraPosition = camera.getPosition();
    std::cout << "Pozice kamery: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;

    // Vol�n� metody onKeyboardEvent t��dy Projectile
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

    //auto camera = Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };

    // Load OBJECTS !
    OBJLoader test{ "./assets/obj/sphere_tri_vnt.obj" };
    auto mesh = test.getMesh();

    OBJLoader secondObj{ "./assets/obj/teapot_tri_vnt.obj" };
    auto secondMesh = secondObj.getMesh();
    
    OBJLoader cubeObj{ "./assets/obj/cube_triangles.obj" };
    auto cubeMesh = cubeObj.getMesh();

     // Registrace funkce pro obsluhu kliknut� my��
    glfwSetMouseButtonCallback(window->getWindow(), onMouseClick);

    //auto vertexShaderPath = std::filesystem::path("./assets/shaders/basic.vert");
    auto vertexShaderPath = std::filesystem::path("./assets/shaders/directional.vert");
    //auto fragmentShaderPath = std::filesystem::path("./assets/shaders/basic.frag");
    //auto fragmentShaderPath = std::filesystem::path("./assets/shaders/newLight.frag");
    auto fragmentShaderPath = std::filesystem::path("./assets/shaders/directional.frag");
    auto shader = Shader(vertexShaderPath, fragmentShaderPath);

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    std::cout << shader.ID << std::endl;
    static int prevX = -1;
    static int prevY = -1;
    bool isResettingCursor = false;
    int width, height, centerX, centerY;
    //glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 objectCenter = glm::vec3(0.0f, 0.0f, 0.0f); // P�edpokl�d�me, �e st�ed objektu je na po��tku sou�adnic
    glm::vec3 lightPos = objectCenter + glm::vec3(0.0f, 3.0f, 0.0f); // Sv�tlo bude um�st�no 3 jednotky nad st�edem objektu
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


        POINT p;
        if (GetCursorPos(&p)) {
            // Z�skat rozm�ry okna
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

            // Aktualizovat p�edchoz� pozici pro dal�� iteraci
            prevX = p.x;
            prevY = p.y;

            // Pokud do�lo ke zm�n� pozice, volat onMouseEvent s relativn� zm�nou
            if ((deltaX != 0 || deltaY != 0) && isCursorOutOfCenter) {
                camera.onMouseEvent(deltaX, deltaY, true);
                isResettingCursor = true;
                glfwSetCursorPos(window->getWindow(), centerX, centerY);
            }
        }




        camera.onKeyboardEvent(window->getWindow(), deltaTime); // process keys etc

        // pro osv�tlen� ->
        //glm::vec3 lightPos(1.0f, 1.0f, 1.0f);
        //// Nastaven� uniformn� prom�nn� pro pozici sv�tla ve vertex shaderu
        //int lightPosLoc = glGetUniformLocation(shader.ID, "lightPos");
        //glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        

        // pro "basic.vert" ->
        /*shader.setUniform("projection", camera.getProjectionMatrix());
        shader.setUniform("view", camera.getViewMatrix());
        shader.setUniform("projection", camera.getProjectionMatrix());*/
        // transformace modelu ->
        //glm::mat4 trans = glm::mat4(1.0f);
        ////trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        //trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -5.0f)); // Nastaven� translace (pozice v prostoru)
        //shader.setUniform("transform", trans); // Nastaven� uniformn� prom�nn� pro transformaci ve vertex shaderu

        //textureID = textureInit(texturePath); // na��st texturu a z�skat jej� ID

        // pro "directional.vert" ->
         glm::mat4 model = glm::mat4(1.0f);
        // P�ed�n� prom�nn� pvmMatrix do shaderu
        glm::mat4 pvmMatrix = camera.getProjectionMatrix() * camera.getViewMatrix() * model; // modelMatrix je matice transformace modelu
        shader.setUniform("pvmMatrix", pvmMatrix);
        // P�ed�n� prom�nn� model do shaderu
        shader.setUniform("model", model); // modelMatrix je matice transformace modelu

        // pro "directional.frag" ->
        // P�ed�n� uniformn� prom�nn� tex0 (sampler2D) do shaderu
        int textureUnit = 0; // Ur�ete vhodnou jednotku textury
        glActiveTexture(GL_TEXTURE0 + textureUnit); // Aktivujte texturovac� jednotku
        glBindTexture(GL_TEXTURE_2D, textureID); // Bindujte texturu na texturovac� jednotku
        shader.setUniform("tex0", textureUnit); // Nastavte uniformn� prom�nnou pro sampler2D

        // P�ed�n� uniformn� prom�nn� lightColor (vec4) do shaderu
        glm::vec4 lightColor(1.0f, 1.0f, 1.0f, 1.0f); // Ur�ete barvu sv�tla
        shader.setUniform("lightColor", lightColor); // Nastavte uniformn� prom�nnou pro barvu sv�tla

        // P�ed�n� uniformn� prom�nn� lightPos (vec3) do shaderu
        glm::vec3 lightPos(1.0f, 1.0f, 1.0f); // Ur�ete pozici sv�tla
        shader.setUniform("lightPos", lightPos); // Nastavte uniformn� prom�nnou pro pozici sv�tla

        // P�ed�n� uniformn� prom�nn� camPos (vec3) do shaderu
        glm::vec3 camPos = camera.getPosition(); // Z�skejte pozici kamery
        shader.setUniform("camPos", camPos); // Nastavte uniformn� prom�nnou pro pozici kamery


        mesh.draw(shader);

        // V�dy, kdy� je posun -> p�edat informace shaderu
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 10.0f, 10.0f)); // Nastav� pozici objektu (x, y, z)
        shader.setUniform("model", model);

        secondMesh.draw(shader);

        // Vykreslit kostku
        //drawCube(10.0f, 0.0f, 0.0f, 0.0f);
        //projectile.drawCube3();
        
        //model = glm::mat4(1.0f);
        ////model = glm::translate(model, glm::vec3(-10.0f, -10.0f, -10.0f));
        //model = glm::translate(model, camPos);
        //shader.setUniform("model", model);

        //projectile.drawAllProjectiles(2.0f);

        // Projektily jsou ulo�eny v poli projectiles
        std::queue<Projectile> temporaryQueue = projectile.getAllProjectiles();
        while (!temporaryQueue.empty()) {
            Projectile currentProjectile = temporaryQueue.front(); // Z�skat aktu�ln� projektil z fronty
            temporaryQueue.pop(); // Odstranit aktu�ln� projektil z fronty

            // Upravit pozici projektilu ve shaderu
            shader.setUniform("model", glm::translate(glm::mat4(1.0f), currentProjectile.position));

            // Vykreslit projektil na jeho pozici
            currentProjectile.drawProjectile(currentProjectile.position);
        }


        //cubeMesh.draw(shader);

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
