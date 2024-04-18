// Projectile.cpp
#include "Projectile.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
// Globální zásobník projektilù
std::vector<Projectile> projectileStack;


Projectile::Projectile(glm::vec3 initialPosition)
    : position(initialPosition) {}

//void Projectile::fire(Projectile& projectile, glm::vec3 targetPosition) {
//    // Nastavení pozice støely na cílovou pozici
//    projectile.position = targetPosition;
//    drawProjectile(projectile);
//}


//void drawProjectile(Projectile& projectile) {
//    // Implementace vykreslení projektilu
//    // Zde mùžete vykreslit geometrii projektilu, napøíklad kostku nebo sféru
//    glColor3f(1.0f, 0.0f, 0.0f); // Èervená barva
//    glPushMatrix();
//    glTranslatef(projectile.position.x, projectile.position.y, projectile.position.z);
//    // Vykreslení kostky pro projektil
//    glBegin(GL_QUADS);
//    glVertex3f(-0.5f, -0.5f, -0.5f);
//    glVertex3f(0.5f, -0.5f, -0.5f);
//    glVertex3f(0.5f, 0.5f, -0.5f);
//    glVertex3f(-0.5f, 0.5f, -0.5f);
//    glEnd();
//    glPopMatrix();
//}

void drawProjectile(Projectile& projectile, float size) {
    // Implementace vykreslení projektilu
    // Zde mùžete vykreslit geometrii projektilu, napøíklad kostku nebo sféru
    glColor3f(1.0f, 0.0f, 0.0f); // Èervená barva
    glPushMatrix();
    //glTranslatef(projectile.position.x, projectile.position.y, projectile.position.z);
    glTranslatef(0.0f, 0.0f, 0.0f);
    // Vykreslení kostky pro projektil
    glBegin(GL_QUADS);
    // Pøední stìna
    glVertex3f(-size / 2.0f, -size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, size / 2.0f);
    // Zadní stìna
    glVertex3f(-size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, -size / 2.0f);
    // Horní stìna
    glVertex3f(-size / 2.0f, size / 2.0f, -size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, -size / 2.0f);
    // Dolní stìna
    glVertex3f(-size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, size / 2.0f);
    glVertex3f(-size / 2.0f, -size / 2.0f, size / 2.0f);
    // Pravá stìna
    glVertex3f(size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, -size / 2.0f);
    glVertex3f(size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(size / 2.0f, -size / 2.0f, size / 2.0f);
    // Levá stìna
    glVertex3f(-size / 2.0f, -size / 2.0f, -size / 2.0f);
    glVertex3f(-size / 2.0f, -size / 2.0f, size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, size / 2.0f);
    glVertex3f(-size / 2.0f, size / 2.0f, -size / 2.0f);
    glEnd();
    glPopMatrix();
    glFlush();
}

void drawCube2(float size, float x, float y, float z) {
    // Vypoèítání poloviny velikosti kostky pro pozici støedu
    float halfSize = size / 2.0f;

    // Nastavení pozice støedu kostky
    glTranslatef(x, y, z);

    // Nastavení barvy kostky
    glColor3f(1.0f, 0.0f, 0.0f); // Èervená barva

    // Vykreslení kostky
    glBegin(GL_QUADS);
    // Pøední stìna
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);

    // Zadní stìna
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);

    // Horní stìna
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);

    // Dolní stìna
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);

    // Pravá stìna
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);

    // Levá stìna
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


// Funkce pro vykreslení projektilù jako kostek
void Projectile::drawAllProjectiles(float size) {
    for (const auto& projectile : projectileStack) {
        drawCube2(size, projectile.position.x, projectile.position.y, projectile.position.z);
        /*std::cout << "vykreslil" << std::endl;*/
    }
}

// Funkce pro obsluhu kliknutí myší
void Projectile::onKeyboardEvent(GLFWwindow* window, glm::vec3 _position, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Leve tlacitko mysi bylo stisknuto." << std::endl;
        Projectile newProjectile(_position); // Vytvoø nový objekt Projectile
        projectileStack.emplace_back(newProjectile); // Pøidej nový projektil do zásobníku
        //drawAllProjectiles(10.0f); // Vykresli všechny projektilky jako kostky
    }
}