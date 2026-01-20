#include "GameObjects.h"

// Player
Player::Player() {
    x = 0.0f;
}

void Player::moveLeft() { if (x > -3) x -= 0.5f; }
void Player::moveRight() { if (x < 3) x += 0.5f; }

void Player::draw() const {
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(x, 0, 0);
    drawCube();
    glPopMatrix();
}

// Enemy
Enemy::Enemy() { reset(); }

bool Enemy::update(float speed) {
    z += speed;
    if (z > 2) {
        reset();
        return true;  // Enemy passed the player
    }
    return false;
}

void Enemy::reset() {
    z = -20.0f;
    x = (rand() % 7) - 3;
}

void Enemy::draw() const {
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(x, 0, z);
    drawCube();
    glPopMatrix();
}

// Utility Functions
void drawCube() {
    glBegin(GL_QUADS);
    // Front
    glVertex3f(-0.5, -0.5,  0.5);
    glVertex3f( 0.5, -0.5,  0.5);
    glVertex3f( 0.5,  0.5,  0.5);
    glVertex3f(-0.5,  0.5,  0.5);
    // Back
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5,  0.5, -0.5);
    glVertex3f( 0.5,  0.5, -0.5);
    glVertex3f( 0.5, -0.5, -0.5);
    // Left
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5,  0.5);
    glVertex3f(-0.5,  0.5,  0.5);
    glVertex3f(-0.5,  0.5, -0.5);
    // Right
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5,  0.5, -0.5);
    glVertex3f(0.5,  0.5,  0.5);
    glVertex3f(0.5, -0.5,  0.5);
    // Top
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5,  0.5);
    glVertex3f( 0.5, 0.5,  0.5);
    glVertex3f( 0.5, 0.5, -0.5);
    // Bottom
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f( 0.5, -0.5, -0.5);
    glVertex3f( 0.5, -0.5,  0.5);
    glVertex3f(-0.5, -0.5,  0.5);
    glEnd();
}

void drawText(float x, float y, const char* text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
