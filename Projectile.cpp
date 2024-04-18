// Projectile.cpp
#include "Projectile.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
// Glob�ln� z�sobn�k projektil�
std::vector<Projectile> projectileStack;


Projectile::Projectile(glm::vec3 initialPosition)
    : position(initialPosition) {}

//void Projectile::fire(Projectile& projectile, glm::vec3 targetPosition) {
//    // Nastaven� pozice st�ely na c�lovou pozici
//    projectile.position = targetPosition;
//    drawProjectile(projectile);
//}


//void drawProjectile(Projectile& projectile) {
//    // Implementace vykreslen� projektilu
//    // Zde m��ete vykreslit geometrii projektilu, nap��klad kostku nebo sf�ru
//    glColor3f(1.0f, 0.0f, 0.0f); // �erven� barva
//    glPushMatrix();
//    glTranslatef(projectile.position.x, projectile.position.y, projectile.position.z);
//    // Vykreslen� kostky pro projektil
//    glBegin(GL_QUADS);
//    glVertex3f(-0.5f, -0.5f, -0.5f);
//    glVertex3f(0.5f, -0.5f, -0.5f);
//    glVertex3f(0.5f, 0.5f, -0.5f);
//    glVertex3f(-0.5f, 0.5f, -0.5f);
//    glEnd();
//    glPopMatrix();
//}

void drawProjectile(Projectile& projectile, float size) {
    // Implementace vykreslen� projektilu
    // Zde m��ete vykreslit geometrii projektilu, nap��klad kostku nebo sf�ru
    glColor3f(1.0f, 0.0f, 0.0f); // �erven� barva
    glPushMatrix();
    //glTranslatef(projectile.position.x, projectile.position.y, projectile.position.z);
    glTranslatef(0.0f, 0.0f, 0.0f);
    // Vykreslen� kostky pro projektil
    glBegin(GL_QUADS);
    // P�edn� st�na
    glVertex3f(-size / 2.0f, -size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, size / 2.0f);
    // Zadn� st�na
    glVertex3f(-size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, -size / 2.0f);
    // Horn� st�na
    glVertex3f(-size / 2.0f, size / 2.0f, -size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, -size / 2.0f);
    // Doln� st�na
    glVertex3f(-size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, size / 2.0f);
    glVertex3f(-size / 2.0f, -size / 2.0f, size / 2.0f);
    // Prav� st�na
    glVertex3f(size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, size / 2.0f);
    // Lev� st�na
    glVertex3f(-size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(-size / 2.0f, -size / 2.0f, size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, -size / 2.0f);
    glEnd();
    glPopMatrix();
    glFlush();
}

void drawCube2(float size, float x, float y, float z) {
    // Vypo��t�n� poloviny velikosti kostky pro pozici st�edu
    float halfSize = size / 2.0f;

    // Nastaven� pozice st�edu kostky
    glTranslatef(x, y, z);

    // Nastaven� barvy kostky
    glColor3f(1.0f, 0.0f, 0.0f); // �erven� barva

    // Vykreslen� kostky
    glBegin(GL_QUADS);
    // P�edn� st�na
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);

    // Zadn� st�na
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);

    // Horn� st�na
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);

    // Doln� st�na
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);

    // Prav� st�na
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);

    // Lev� st�na
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glEnd();
    glFlush();
}

void Projectile::drawCube3() {
    //drawCube2(10.0f, 0.0f, 0.0f, 0.0f);
}


// Funkce pro vykreslen� projektil� jako kostek
void Projectile::drawAllProjectiles(float size) {
    for (const auto& projectile : projectileStack) {
        drawCube2(size, projectile.position.x, projectile.position.y, projectile.position.z);
        /*std::cout << "vykreslil" << std::endl;*/
    }
}

// Funkce pro obsluhu kliknut� my��
void Projectile::onKeyboardEvent(GLFWwindow* window, glm::vec3 _position, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Leve tlacitko mysi bylo stisknuto." << std::endl;
        Projectile newProjectile(_position); // Vytvo� nov� objekt Projectile
        projectileStack.emplace_back(newProjectile); // P�idej nov� projektil do z�sobn�ku
        //drawAllProjectiles(10.0f); // Vykresli v�echny projektilky jako kostky
    }
}