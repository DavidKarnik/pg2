#pragma once
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <opencv2\opencv.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

class Texture {
public:
    Texture() {};
};

// generate GL texture from image file
GLuint TextureInit(const char* filepath);

// generate GL texture from OpenCV image
GLuint TextureGen(cv::Mat& image);

#endif