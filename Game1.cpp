#include "game1.h"
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>


Snake snake;
Food food;
bool gameOver = false;
int score = 0;


static int frameCount = 0;
static const int MOVE_DELAY = 18;


static void setupSnakeProjection()
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


static void drawScreenText(float x, float y, const char* text)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 900, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);
    for (int i = 0; text[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


Snake::Snake()
{
    length = 3;
    dir = 1;

    for (int i = 0; i < length; i++) {
        x[i] = -i;
        y[i] = 0;
    }
}

void Snake::update()
{
    for (int i = length - 1; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    if (dir == 0) y[0]++;
    if (dir == 1) x[0]++;
    if (dir == 2) y[0]--;
    if (dir == 3) x[0]--;
}

void Snake::draw() const
{
    for (int i = 0; i < length; i++) {
        if (i == 0) glColor3f(0.0f, 1.0f, 0.5f);
        else        glColor3f(0.0f, 0.8f, 0.0f);
        drawBlock(x[i], y[i]);
    }
}


Food::Food() { respawn(); }

void Food::respawn()
{
    x = (rand() % 18) - 9;
    y = (rand() % 18) - 9;
}

void Food::draw() const
{
    glColor3f(1, 0, 0);
    drawBlock(x, y);
}


void drawBlock(float x, float y)
{
    glBegin(GL_QUADS);
        glVertex2f(x + 0.1f, y + 0.1f);
        glVertex2f(x + 0.9f, y + 0.1f);
        glVertex2f(x + 0.9f, y + 0.9f);
        glVertex2f(x + 0.1f, y + 0.9f);
    glEnd();
}

void drawGrid()
{
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++) {
        glVertex2f(i, -10); glVertex2f(i, 10);
        glVertex2f(-10, i); glVertex2f(10, i);
    }
    glEnd();
}


void initGame1()
{
    gameOver = false;
    score = 0;
    frameCount = 0;
    snake = Snake();
    food.respawn();
}

void displayGame1()
{
    glClear(GL_COLOR_BUFFER_BIT);
    setupSnakeProjection();

    drawGrid();

    // Border
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-10, -10);
        glVertex2f( 10, -10);
        glVertex2f( 10,  10);
        glVertex2f(-10,  10);
    glEnd();

    snake.draw();
    food.draw();


    char buf[64];
    sprintf(buf, "Score: %d", score);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawScreenText(20, 560, buf);


    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawScreenText(360, 320, "GAME OVER");

        glColor3f(0.0f, 0.0f, 0.0f);
        sprintf(buf, "Final Score: %d", score);
        drawScreenText(350, 280, buf);

        glColor3f(0.0f, 0.0f, 0.0f);   //black
        drawScreenText(330, 240, "Press R to Restart");
        drawScreenText(330, 200, "Press H to return to Hub");
    }

    glutSwapBuffers();
}

void updateGame1(int)
{
    if (!gameOver) {
        frameCount++;

        if (frameCount % MOVE_DELAY == 0) {
            snake.update();

            // Eat food
            if ((int)snake.x[0] == (int)food.x &&
                (int)snake.y[0] == (int)food.y) {
                snake.length++;
                food.respawn();
                score++;
            }

            // Boundary collision
            if (snake.x[0] < -10 || snake.x[0] >= 10 ||
                snake.y[0] < -10 || snake.y[0] >= 10) {
                gameOver = true;
            }
        }
    }

    glutPostRedisplay();
}


void keyboardGame1(unsigned char key, int, int)
{
    if (!gameOver) {
        if ((key=='w'||key=='W') && snake.dir!=2) snake.dir=0;
        if ((key=='d'||key=='D') && snake.dir!=3) snake.dir=1;
        if ((key=='s'||key=='S') && snake.dir!=0) snake.dir=2;
        if ((key=='a'||key=='A') && snake.dir!=1) snake.dir=3;
    }

    if (gameOver && (key=='r' || key=='R')) {
        initGame1();
    }
    extern int currentGame;//

if (gameOver && (key == 'h' || key == 'H')) {
    currentGame = 0;
}
}

void keyboardUpGame1(unsigned char, int, int) {}
void specialKeyGame1(int, int, int) {}
void reshapeGame1(int w, int h)
{
    glViewport(0, 0, w, h);
}

