// Projectile.cpp
#include "Projectile.h"
#include <GLFW/glfw3.h>
#include <iostream>
//#include <vector>
#include <queue>
// Glob�ln� z�sobn�k projektil�
std::queue<Projectile> projectileQueue;


Projectile::Projectile(glm::vec3 initialPosition)
    : position(initialPosition) {}

//void Projectile::fire(Projectile& projectile, glm::vec3 targetPosition) {
//    // Nastaven� pozice st�ely na c�lovou pozici
//    projectile.position = targetPosition;
//    drawProjectile(projectile);
//}


void drawCube2(float size, float x, float y, float z) {
    std::cout << "Vykreslen�: (" << x << ", " << y << ", " << z << ")" << std::endl;
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
    //glPopMatrix();
    glFlush();
}


void addProjectile(const glm::vec3& position) {
    Projectile newProjectile(position);
    projectileQueue.push(newProjectile);
}

//void Projectile::drawAllProjectiles(float size) {
//    while (!projectileQueue.empty()) {
//        const Projectile& projectile = projectileQueue.front();
//        drawCube2(size, projectile.position.x, projectile.position.y, projectile.position.z);
//        projectileQueue.pop();
//    }
//}

void Projectile::drawAllProjectiles(float size) {
    //
    // TODO REPAIR MOVEMENT -> NOT MOVING :(
    // 
    // Vytvo��me kopii fronty
    std::queue<Projectile> temporaryQueue = projectileQueue;
    // Simulace pohybu projektil�
    glm::vec3 movement(10.0f, 0.0f, 0.0f); // Pohyb o 0.1 na osu x
    Projectile projectile; // Vytvo�en� objektu Projectile
    // Projdeme v�echny projektilov� objekty ve front�
    while (!temporaryQueue.empty()) {
        // Z�sk�n� projektilu z fronty
        projectile = projectileQueue.front();
        // Posun projektilu o vektor movement
        projectile.position += movement;

        projectile = temporaryQueue.front();
        
        drawCube2(size, projectile.position.x, projectile.position.y, projectile.position.z);

        temporaryQueue.pop(); // Neprov�d�me pop na p�vodn� front�
    }
}

void Projectile::drawProjectile(glm::vec3 _position) {
    float size = 2.0f;
    drawCube2(size, _position.x, _position.y, _position.z);
}


void removeFirstProjectile() {
    if (!projectileQueue.empty()) {
        projectileQueue.pop();
    }
}

// Funkce pro obsluhu kliknut� my��
void Projectile::onKeyboardEvent(GLFWwindow* window, glm::vec3 _position, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Leve tlacitko mysi bylo stisknuto." << std::endl;
        Projectile newProjectile(_position); // Vytvo� nov� objekt Projectile
        std::cout << "_position: (" << _position.x << ", " << _position.y << ", " << _position.z << ")" << std::endl;
        //projectileStack.emplace_back(newProjectile); // P�idej nov� projektil do z�sobn�ku
        //drawAllProjectiles(10.0f); // Vykresli v�echny projektilky jako kostky
        addProjectile(_position);
    }
}

std::queue<Projectile> Projectile::getAllProjectiles() {
    return projectileQueue;
}