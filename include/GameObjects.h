#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

// Player Class
class Player {
public:
    float x;
    Player();
    void moveLeft();
    void moveRight();
    void draw() const;
};

// Enemy Class
class Enemy {
public:
    float x, z;
    Enemy();
    bool update(float speed);  // Returns true if passed player
    void reset();
    void draw() const;
};

// Utility Functions
void drawCube();
void drawText(float x, float y, const char* text);

#endif
