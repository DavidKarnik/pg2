#pragma once
#include "Window.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <opencv2/opencv.hpp>

#include "Model.h"
#include "Shader.h"
#include "Camera.h"

class App {
public:
    App();

    bool init(void);
    void report(void);
    int run(void);

    void InitAssets();
    void CreateModel(std::string name, std::string obj, std::string tex, bool is_opaque, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);
    void UpdateModels();
   
    static Camera camera;
    glm::vec3 rgb_orange = { 1.0f, 0.5f, 0.0f };
    glm::vec3 rgb_white = { 1.0f, 1.0f, 1.0f };
    glm::vec4 rgba_white = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Set light position
    glm::vec3 light_position = { -100000, 0, 100000 };
    glm::mat4 mx_projection = glm::identity<glm::mat4>();

    Shader shader;

    ~App();
private:
    Window* window;

    std::map<std::string, Model> scene_opaque;
    std::map<std::string, Model> scene_transparent;

    static bool is_vsync_on;
    static bool is_fullscreen_on;

    
protected:
    //Camera camera = Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };
    // Maze
    ///*
    uchar MapGet(cv::Mat& map, int x, int y);
    void MazeGenerate(cv::Mat& map);
    /**/
};
