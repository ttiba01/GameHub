#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

#define MAX_CARS 6
#define MIN_GAP 0.35f

float lanes[4] = { -0.75f, -0.25f, 0.25f, 0.75f };

float playerX = 0.0f;
float playerY = -0.7f;


float carX[MAX_CARS];
float carY[MAX_CARS];

bool gameOver = false;
float speed = 0.01f;
int frameCount = 0;


float roadOffset = 0.0f;


void drawCircle(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.1416f / 180;
        glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
    }
    glEnd();
}


void drawCar(float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);


    glBegin(GL_QUADS);
        glVertex2f(x - 0.07f, y - 0.12f);
        glVertex2f(x + 0.07f, y - 0.12f);
        glVertex2f(x + 0.07f, y + 0.12f);
        glVertex2f(x - 0.07f, y + 0.12f);
    glEnd();


    glColor3f(0.8f, 0.9f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x - 0.05f, y + 0.02f);
        glVertex2f(x + 0.05f, y + 0.02f);
        glVertex2f(x + 0.04f, y + 0.10f);
        glVertex2f(x - 0.04f, y + 0.10f);
    glEnd();


    glColor3f(0, 0, 0);
    drawCircle(x - 0.05f, y - 0.10f, 0.02f);
    drawCircle(x + 0.05f, y - 0.10f, 0.02f);
}


void drawRoad() {
    glColor3f(1, 1, 1);
    for (float y = -1.0f; y < 1.2f; y += 0.2f) {
        glBegin(GL_QUADS);
            glVertex2f(-0.02f, y + roadOffset);
            glVertex2f( 0.02f, y + roadOffset);
            glVertex2f( 0.02f, y + 0.1f + roadOffset);
            glVertex2f(-0.02f, y + 0.1f + roadOffset);
        glEnd();
    }
}

bool tooCloseInLane(int laneIndex, float newY) {
    for (int i = 0; i < MAX_CARS; i++) {
        if (carX[i] == lanes[laneIndex]) {
            if (fabs(carY[i] - newY) < MIN_GAP)
                return true;
        }
    }
    return false;
}


void resetCar(int i) {
    int lane;
    float y;

    do {
        lane = rand() % 4;
        y = 1.2f + (rand() % 100) / 100.0f;
    }
    while (tooCloseInLane(lane, y));

    carX[i] = lanes[lane];
    carY[i] = y;
}


void checkCollision() {
    for (int i = 0; i < MAX_CARS; i++) {
        if (fabs(playerX - carX[i]) < 0.12f &&
            fabs(playerY - carY[i]) < 0.18f)
            gameOver = true;
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawRoad();

    if (!gameOver) {


        glPushMatrix();
            glTranslatef(playerX, playerY, 0);
            glRotatef(180, 0, 0, 1);
            drawCar(0, 0, 0.0f, 0.8f, 1.0f);
        glPopMatrix();


        for (int i = 0; i < MAX_CARS; i++)
            drawCar(carX[i], carY[i], 1, 0, 0);
    }

    glutSwapBuffers();
}


void update(int value) {
    if (!gameOver) {
        frameCount++;

        roadOffset -= speed;
        if (roadOffset < -0.2f)
            roadOffset = 0.0f;


        if (frameCount % 200 == 0)
            speed += 0.002f;

        for (int i = 0; i < MAX_CARS; i++) {
            carY[i] -= speed;
            if (carY[i] < -1.2f)
                resetCar(i);
        }

        checkCollision();
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 'a') playerX -= 0.1f;
    if (key == 'd') playerX += 0.1f;

    //if (key == 'w') playerY += 0.05f;
    //if (key == 's') playerY -= 0.05f;


    if (key == 'r') {
        gameOver = false;
        speed = 0.01f;
        frameCount = 0;
        roadOffset = 0.0f;
        playerX = 0.0f;

        for (int i = 0; i < MAX_CARS; i++)
            resetCar(i);
    }
}


void init() {
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    for (int i = 0; i < MAX_CARS; i++)
        resetCar(i);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("2D Car Game - 4 Lanes Driving");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
