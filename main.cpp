#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

enum GameState {
    MENU,
    PLAYING,
    PAUSE,
    GAMEOVER
};

GameState currentState = MENU;

float playerX = 0.0f;

struct Enemy {
    float x;
    float z;
};

const int ENEMY_COUNT = 2;
Enemy enemies[ENEMY_COUNT];
float enemySpeed = 0.2f;

int score = 0;

void drawText(float x, float y, const char* text)
{
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

void drawCube()
{
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

void initEnemies()
{
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemies[i].z = -20.0f - i * 8.0f;
        enemies[i].x = (rand() % 7) - 3;
    }
}

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

    glLoadIdentity();
    gluLookAt(0, 3, 5,
              0, 0, -10,
              0, 1, 0);

    glColor3f(0, 0, 1);
    glPushMatrix();
        glTranslatef(playerX, 0, 0);
        drawCube();
    glPopMatrix();

    glColor3f(1, 0, 0);
    for (int i = 0; i < ENEMY_COUNT; i++) {
        glPushMatrix();
            glTranslatef(enemies[i].x, 0, enemies[i].z);
            drawCube();
        glPopMatrix();
    }

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
            enemies[i].z += enemySpeed;

            if (enemies[i].z > 2) {
                enemies[i].z = -20;
                enemies[i].x = (rand() % 7) - 3;
                score += 10;
            }

            if (fabs(playerX - enemies[i].x) < 0.8f &&
                enemies[i].z > -1 && enemies[i].z < 1) {
                currentState = GAMEOVER;
            }
        }

        enemySpeed += 0.00005f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void specialKey(int key, int, int)
{
    if (currentState == PLAYING) {
        if (key == GLUT_KEY_LEFT && playerX > -3)
            playerX -= 0.5f;
        if (key == GLUT_KEY_RIGHT && playerX < 3)
            playerX += 0.5f;
    }
}

void keyboard(unsigned char key, int, int)
{
    if (currentState == MENU) {
        if (key == 's' || key == 'S') {
            initEnemies();
            score = 0;
            enemySpeed = 0.2f;
            playerX = 0;
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
            initEnemies();
            score = 0;
            enemySpeed = 0.2f;
            playerX = 0;
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
