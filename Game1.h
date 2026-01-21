#ifndef GAME1_H
#define GAME1_H

#include <GL/glut.h>


class Snake {
public:
    int length;
    float x[100], y[100];
    int dir; // 0=up, 1=right, 2=down, 3=left

    Snake();
    void update();
    void draw() const;
};


class Food {
public:
    float x, y;

    Food();
    void respawn();
    void draw() const;
};


void initGame1();
void displayGame1();
void updateGame1(int);
void keyboardGame1(unsigned char key, int x, int y); // WASD movement
void keyboardUpGame1(unsigned char key, int x, int y); // optional
void specialKeyGame1(int key, int x, int y);         // optional (arrow keys backup)


void drawBlock(float x, float y);
void drawGrid();
void drawText(float x, float y, const char* text);
void reshapeGame1(int w, int h);

#endif
