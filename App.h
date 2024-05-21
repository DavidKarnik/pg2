#pragma once

#include <map>

#include <opencv2/opencv.hpp>

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Window.h"
#include "Projectile.h"
#include <irrKlang.h>
#include "Audio.h"

//bool App::holdItem = false;

#define PLAYER_HEIGHT 1.0f      // Camera above ground
#define HEIGHTMAP_SHIFT 60.0f   // Heightmap is shifted by this value on x and z coordinates
#define N_PROJECTILES 10        // How many projectiles are there in the pool
#define HEGHTMAP_SCALE 0.1f

class App {
public:
    App();

    bool Init();
    void InitAssets();
    int Run();
    //void CreateModel(std::string name, std::string obj, std::string tex, bool is_opaque, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);
    //Model* CreateModel(std::string name, std::string obj, std::string tex, bool is_opaque, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);
    Model* CreateModel(std::string name, std::string obj, std::string tex, bool is_opaque, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation);

    void UpdateModels();
    void RemoveModel(std::string name);

    void holdNewItem();
    Model* findClosestModel(glm::vec3& cameraPosition);
    Model* findClosestModelInItemPickUpRange(glm::vec3& cameraPosition);
    Model* findHeldItem();

    irrklang::ISoundEngine* soundEngine;

    Audio audio;
    Model* sound_model{};
    glm::vec2 sound_to_player{};
    glm::vec2 last_sound_to_player{};

    ~App();
private:

    /*std::map<std::string, Model> scene_opaque;
    std::map<std::string, Model> scene_transparent;*/
    std::map<std::string, Model*> scene_opaque;
    std::map<std::string, Model*> scene_transparent;
    // for sorting - from nearest to most far .. transparent obj
    //std::vector<std::pair<const std::string, Model>*> scene_transparent_pairs; 
    std::vector<std::pair<const std::string, Model*>*> scene_transparent_pairs; 

    static Projectile projectile;

    static bool is_vsync_on;
    static bool is_fullscreen_on;

    static float itemPickUpRange;
    static bool holdItem;

    static bool isFlashlightOn;
    static bool isLampOn;

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
    //glm::vec4 clear_color = glm::vec4(10.0f/255, 30.0f/255, 50.0f/255, 0.0f);

    void UpdateProjectionMatrix();

    void GetInformation();

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    //static void key_callback_items(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    Shader shader;

    void CreateWaterDrops(glm::vec3 _cloudPosition);
    void UpdateWaterDrops(glm::vec3 _cloudPosition);

    // Heightmap
    std::map<std::pair<float, float>, float>* _heights{};
    float GetHeightmapY(float position_x, float position_z) const;

    // Maze
    ///*
    uchar MapGet(cv::Mat& map, int x, int y);
    void MazeGenerate(cv::Mat& map);
    /**/
};