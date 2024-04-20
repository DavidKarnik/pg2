#pragma once

#include <opencv2\opencv.hpp>
#include <GL/glew.h>
class Texture {
public:
    Texture() {};
};

// moje
GLuint loadTexture(const std::string& filePath);

// generate GL texture from image file
GLuint textureInit(const char* filepath);

// generate GL texture from OpenCV image
GLuint tex_gen(cv::Mat& image);
