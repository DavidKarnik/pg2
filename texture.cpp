//#include <opencv2\opencv.hpp>
//#include "texture.h"

// moje
#include <iostream>
#include <GL/glew.h>
#include <opencv2/opencv.hpp>

GLuint loadTexture(const std::string& filePath) {
    // Na�ten� obr�zku pomoc� OpenCV
    cv::Mat image = cv::imread(filePath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    // Generov�n� a v�z�n� textury v OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Nastaven� parametr� textury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Nahr�n� dat textury do OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Uvoln�n� pam�ti a vr�cen� identifik�toru textury
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}
