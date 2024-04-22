
#include <iostream>
#include <chrono>

// OpenCV 
//#include <opencv2\opencv.hpp>
#include <GL/glew.h> 

// OpenGL Extension Wrangler
#include <GL/wglew.h> //WGLEW = Windows GL Extension Wrangler (change for different platform) 

// GLFW toolkit
#include <GLFW/glfw3.h>

// OpenGL math
#include <glm/glm.hpp>

#include "App.h"

// Define instance of our app

App app;

int main()
{
    if (app.Init()) {
        return app.Run();
    }
}
