// Projectile.h
#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Projectile {
public:
    glm::vec3 position;
    //std::vector<Projectile> projectileStack;

    Projectile(glm::vec3 initialPosition);

    //void fire(Projectile& projectile, glm::vec3 targetPosition);
    void drawCube3();
    void drawAllProjectiles(float size);
    void onKeyboardEvent(GLFWwindow* window, glm::vec3 _position, int button, int action, int mods);
};
