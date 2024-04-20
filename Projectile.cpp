// Projectile.cpp
#include "Projectile.h"
#include <GLFW/glfw3.h>
#include <iostream>
//#include <vector>
#include <queue>
// Globální zásobník projektilù
std::queue<Projectile> projectileQueue;


Projectile::Projectile(glm::vec3 initialPosition)
    : position(initialPosition) {}

//void Projectile::fire(Projectile& projectile, glm::vec3 targetPosition) {
//    // Nastavení pozice støely na cílovou pozici
//    projectile.position = targetPosition;
//    drawProjectile(projectile);
//}


void drawCube2(float size, float x, float y, float z) {
    std::cout << "Vykreslení: (" << x << ", " << y << ", " << z << ")" << std::endl;
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
    // Vytvoøíme kopii fronty
    std::queue<Projectile> temporaryQueue = projectileQueue;
    // Simulace pohybu projektilù
    glm::vec3 movement(10.0f, 0.0f, 0.0f); // Pohyb o 0.1 na osu x
    Projectile projectile; // Vytvoøení objektu Projectile
    // Projdeme všechny projektilové objekty ve frontì
    while (!temporaryQueue.empty()) {
        // Získání projektilu z fronty
        projectile = projectileQueue.front();
        // Posun projektilu o vektor movement
        projectile.position += movement;

        projectile = temporaryQueue.front();
        
        drawCube2(size, projectile.position.x, projectile.position.y, projectile.position.z);

        temporaryQueue.pop(); // Neprovádíme pop na pùvodní frontì
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

// Funkce pro obsluhu kliknutí myší
void Projectile::onKeyboardEvent(GLFWwindow* window, glm::vec3 _position, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Leve tlacitko mysi bylo stisknuto." << std::endl;
        Projectile newProjectile(_position); // Vytvoø nový objekt Projectile
        std::cout << "_position: (" << _position.x << ", " << _position.y << ", " << _position.z << ")" << std::endl;
        //projectileStack.emplace_back(newProjectile); // Pøidej nový projektil do zásobníku
        //drawAllProjectiles(10.0f); // Vykresli všechny projektilky jako kostky
        addProjectile(_position);
    }
}

std::queue<Projectile> Projectile::getAllProjectiles() {
    return projectileQueue;
}