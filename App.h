#pragma once
#include "Window.h"

#pragma once

#include <unordered_map>

#include <opencv2/opencv.hpp>

#include "Model.h"
#include "Shader.h"
#include "Camera.h"

class App {
public:
    App();

    bool Init();
    void InitAssets();
    int Run();
    void CreateModel(std::string name, std::string obj, std::string tex, bool is_opaque, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);
    void UpdateModels();

    ~App();
private:
    std::map<std::string, Model> scene_opaque;
    std::map<std::string, Model> scene_transparent;

    static bool is_vsync_on;
    static bool is_fullscreen_on;

    static GLFWmonitor* monitor;
    static const GLFWvidmode* mode;
    static int window_xcor;
    static int window_ycor;
    static int window_width;
    static int window_height;
    static int window_width_return_from_fullscreen;
    static int window_height_return_from_fullscreen;

    float FOV = 89.0f;
    glm::mat4 mx_projection = glm::identity<glm::mat4>();
    static Camera camera;
    static double last_cursor_xpos;
    static double last_cursor_ypos;

    GLFWwindow* window = nullptr;
    glm::vec4 clear_color = glm::vec4(0, 0, 0, 0);

    void UpdateProjectionMatrix();

    void GetInformation();

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods, float deltaTime);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    Shader shader;

    // Maze
    ///*
    uchar MapGet(cv::Mat& map, int x, int y);
    void MazeGenerate(cv::Mat& map);
    /**/
};