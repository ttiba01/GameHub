#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "GameObjects.h"

enum GameState { MENU, PLAYING, PAUSE, GAMEOVER };
GameState currentState = MENU;

Player player;
const int ENEMY_COUNT = 2;
Enemy enemies[ENEMY_COUNT];

float enemySpeed = 0.2f;
int score = 0;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentState == MENU) {
        glColor3f(0, 0, 0);
        drawText(330, 360, "CUBE DODGE GAME");
        drawText(360, 300, "S - START");
        drawText(360, 260, "Q - QUIT");
        glutSwapBuffers();
        return;
    }

    if (currentState == PAUSE) {
        glColor3f(0, 0, 0);
        drawText(360, 320, "PAUSED");
        drawText(320, 280, "R - RESUME");
        drawText(320, 240, "Q - QUIT");
        glutSwapBuffers();
        return;
    }

    if (currentState == GAMEOVER) {
        glColor3f(0, 0, 0);
        drawText(350, 320, "GAME OVER");
        drawText(320, 280, "R - RESTART");
        drawText(320, 240, "Q - QUIT");
        glutSwapBuffers();
        return;
    }

    // 3D Camera setup
    glLoadIdentity();
    gluLookAt(0, 3, 5,
              0, 0, -10,
              0, 1, 0);

    // Drawing player
    player.draw();

    // Drawing enemies
    for (int i = 0; i < ENEMY_COUNT; i++)
        enemies[i].draw();

    // Drawing score
    char buf[40];
    sprintf(buf, "Score: %d", score);
    glColor3f(0, 0, 0);
    drawText(20, 560, buf);

    glutSwapBuffers();
}

void update(int)
{
    if (currentState == PLAYING) {

        for (int i = 0; i < ENEMY_COUNT; i++) {
    if (enemies[i].update(enemySpeed)) {
        score += 10;
    }

    // Collision detection
    if (fabs(player.x - enemies[i].x) < 0.8f &&
        enemies[i].z > -1 && enemies[i].z < 1) {
        currentState = GAMEOVER;
    }
}

        enemySpeed += 0.00009f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Arrow Key Input
void specialKey(int key, int, int)
{
    if (currentState == PLAYING) {
        if (key == GLUT_KEY_LEFT)
            player.moveLeft();
        if (key == GLUT_KEY_RIGHT)
            player.moveRight();
    }
}

void keyboard(unsigned char key, int, int)
{
    if (currentState == MENU) {
        if (key == 's' || key == 'S') {
            for (int i = 0; i < ENEMY_COUNT; i++) enemies[i].reset();
            score = 0;
            enemySpeed = 0.2f;
            player.x = 0;
            currentState = PLAYING;
        }
        if (key == 'q' || key == 'Q')
            exit(0);
    }
    else if (currentState == PAUSE) {
        if (key == 'r' || key == 'R')
            currentState = PLAYING;
        if (key == 'q' || key == 'Q')
            exit(0);
    }
    else if (currentState == GAMEOVER) {
        if (key == 'r' || key == 'R') {
            for (int i = 0; i < ENEMY_COUNT; i++) enemies[i].reset();
            score = 0;
            enemySpeed = 0.2f;
            player.x = 0;
            currentState = PLAYING;
        }
        if (key == 'q' || key == 'Q')
            exit(0);
    }

    // Pause during play
    if (currentState == PLAYING && (key == 'p' || key == 'P'))
        currentState = PAUSE;
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 800.0 / 600.0, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cube Dodge Game");

    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
