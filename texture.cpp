//#include <opencv2\opencv.hpp>
//#include "texture.h"

// moje
#include <iostream>
#include <GL/glew.h>
#include <opencv2/opencv.hpp>

GLuint loadTexture(const std::string& filePath) {
    // Naètení obrázku pomocí OpenCV
    cv::Mat image = cv::imread(filePath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    // Generování a vázání textury v OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Nastavení parametrù textury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Nahrání dat textury do OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Uvolnìní pamìti a vrácení identifikátoru textury
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}
