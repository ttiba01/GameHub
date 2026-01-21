#include <GL/glut.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

const float PI = 3.14159265359f;

float clamp01(float a) {
    if (a < 0) return 0;
    if (a > 1) return 1;
    return a;
}

bool insideRect(float px, float py, float x, float y, float w, float h) {
    return (px >= x && px <= x + w && py >= y && py <= y + h);
}

void drawText(float x, float y, const string& s, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    for (int i = 0; i < (int)s.size(); i++) {
        glutBitmapCharacter(font, s[i]);
    }
}

void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);
    glEnd();
}

void drawRectOutline(float x, float y, float w, float h, float lineW = 2.0f) {
    glLineWidth(lineW);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);
    glEnd();
}

void drawCircle(float cx, float cy, float r, int seg = 32) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= seg; i++) {
        float t = (2.0f * PI * i) / seg;
        glVertex2f(cx + cos(t) * r, cy + sin(t) * r);
    }
    glEnd();
}


void drawCardPanel(float x, float y, float w, float h,
                   float r, float g, float b,
                   bool glow)
{
    // Shadow
    glColor3f(0, 0, 0);
    drawRect(x + 0.015f, y - 0.015f, w, h);

    // Glow (hover)
    if (glow) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 0.12f);
        drawRect(x - 0.01f, y - 0.01f, w + 0.02f, h + 0.02f);
        glDisable(GL_BLEND);
    }

    // Main body
    glColor3f(r, g, b);
    drawRect(x, y, w, h);

    // Highlight strip
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1, 1, 1, 0.12f);
    drawRect(x, y + h * 0.55f, w, h * 0.45f);
    glDisable(GL_BLEND);

    // Border
    glColor3f(0.15f, 0.15f, 0.15f);
    drawRectOutline(x, y, w, h, 2.0f);
}

void drawStar(float cx, float cy, float outerR, float innerR) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 10; i++) {
        float ang = (PI / 5.0f) * i - PI / 2.0f;
        float r = (i % 2 == 0) ? outerR : innerR;
        glVertex2f(cx + cos(ang) * r, cy + sin(ang) * r);
    }
    glEnd();
}

void drawHeart(float cx, float cy, float s) {
    drawCircle(cx - s * 0.22f, cy + s * 0.12f, s * 0.22f, 24);
    drawCircle(cx + s * 0.22f, cy + s * 0.12f, s * 0.22f, 24);

    glBegin(GL_TRIANGLES);
    glVertex2f(cx - s * 0.44f, cy + s * 0.10f);
    glVertex2f(cx + s * 0.44f, cy + s * 0.10f);
    glVertex2f(cx,             cy - s * 0.55f);
    glEnd();
}

void drawDiamond(float cx, float cy, float s) {
    glBegin(GL_QUADS);
    glVertex2f(cx,     cy + s);
    glVertex2f(cx + s, cy);
    glVertex2f(cx,     cy - s);
    glVertex2f(cx - s, cy);
    glEnd();
}

void drawClover(float cx, float cy, float s) {
    drawCircle(cx - s * 0.25f, cy + s * 0.10f, s * 0.22f, 24);
    drawCircle(cx + s * 0.25f, cy + s * 0.10f, s * 0.22f, 24);
    drawCircle(cx,             cy - s * 0.18f, s * 0.22f, 24);

    glBegin(GL_QUADS);
    glVertex2f(cx - s * 0.06f, cy - s * 0.55f);
    glVertex2f(cx + s * 0.06f, cy - s * 0.55f);
    glVertex2f(cx + s * 0.06f, cy - s * 0.18f);
    glVertex2f(cx - s * 0.06f, cy - s * 0.18f);
    glEnd();
}

void drawCrown(float cx, float cy, float s) {
    glBegin(GL_QUADS);
    glVertex2f(cx - s * 0.45f, cy - s * 0.35f);
    glVertex2f(cx + s * 0.45f, cy - s * 0.35f);
    glVertex2f(cx + s * 0.45f, cy - s * 0.15f);
    glVertex2f(cx - s * 0.45f, cy - s * 0.15f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(cx - s * 0.45f, cy - s * 0.15f);
    glVertex2f(cx - s * 0.25f, cy + s * 0.35f);
    glVertex2f(cx - s * 0.05f, cy - s * 0.15f);

    glVertex2f(cx - s * 0.10f, cy - s * 0.15f);
    glVertex2f(cx,             cy + s * 0.45f);
    glVertex2f(cx + s * 0.10f, cy - s * 0.15f);

    glVertex2f(cx + s * 0.05f, cy - s * 0.15f);
    glVertex2f(cx + s * 0.25f, cy + s * 0.35f);
    glVertex2f(cx + s * 0.45f, cy - s * 0.15f);
    glEnd();
}

class Card {
public:
    float x, y, w, h;
    int pairID;

    bool matched;
    bool faceUp;

    float flip;       // 0 = back, 1 = face
    float targetFlip; // where we want flip to go

    Card() : x(0), y(0), w(0), h(0), pairID(0),
             matched(false), faceUp(false), flip(0), targetFlip(0) {}

    Card(float X, float Y, float W, float H, int id)
        : x(X), y(Y), w(W), h(H), pairID(id),
          matched(false), faceUp(false), flip(0), targetFlip(0) {}

    bool contains(float px, float py) const {
        return insideRect(px, py, x, y, w, h);
    }

    void setFaceUp(bool up) {
        faceUp = up;
        targetFlip = up ? 1.0f : 0.0f;
    }

    void updateAnim() {
        float speed = 0.18f;
        flip += (targetFlip - flip) * speed;
        if (fabs(targetFlip - flip) < 0.002f) flip = targetFlip;
    }

    void draw(bool hover) const {
        // Flip illusion: scale x
        float sx = fabs(0.5f - flip) * 2.0f;
        sx = clamp01(sx);

        float cx = x + w * 0.5f;
        float cy = y + h * 0.5f;

        glPushMatrix();
        glTranslatef(cx, cy, 0);
        glScalef(sx, 1.0f, 1.0f);
        glTranslatef(-cx, -cy, 0);

        bool showFace = (flip > 0.5f);

        if (matched) drawFace(hover, true);
        else if (showFace) drawFace(hover, false);
        else drawBack(hover);

        glPopMatrix();
    }

private:
    void palette(float &r, float &g, float &b) const {
        switch (pairID) {
            case 0: r=1.00f; g=0.83f; b=0.26f; break; // gold
            case 1: r=1.00f; g=0.35f; b=0.52f; break; // pink
            case 2: r=0.25f; g=0.74f; b=0.96f; break; // cyan
            case 3: r=0.35f; g=0.92f; b=0.58f; break; // green
            case 4: r=0.70f; g=0.50f; b=1.00f; break; // purple
            default:r=1.00f; g=0.55f; b=0.20f; break; // orange
        }
    }

    void drawBack(bool hover) const {
        drawCardPanel(x, y, w, h, 0.25f, 0.05f, 0.08f, hover);
        glColor3f(1,1,1);
        drawText(x + w * 0.20f, y + h * 0.65f, "MEMORY", GLUT_BITMAP_HELVETICA_18);
        drawText(x + w * 0.32f, y + h * 0.50f, "CARD",   GLUT_BITMAP_HELVETICA_18);
    }

    void drawFace(bool hover, bool dim) const {
        float r,g,b;
        palette(r,g,b);
        if (dim) { r*=0.75f; g*=0.75f; b*=0.75f; }

        drawCardPanel(x, y, w, h, r, g, b, hover);

        // Symbol in the middle
        glColor3f(1,1,1);
        float cx = x + w * 0.5f;
        float cy = y + h * 0.52f;
        float s  = w * 0.20f;

        switch (pairID) {
            case 0: drawStar(cx, cy, s * 1.15f, s * 0.50f); break;
            case 1: drawHeart(cx, cy, s * 1.25f); break;
            case 2: drawCircle(cx, cy, s * 0.95f, 40); break;
            case 3: drawClover(cx, cy, s * 1.10f); break;
            case 4: drawDiamond(cx, cy, s * 1.05f); break;
            case 5: drawCrown(cx, cy, s * 1.20f); break;
        }
    }
};



class Board {
public:
    int cols, rows;
    vector<Card> cards;

    Board() : cols(4), rows(3) {}

    void create(float left, float bottom, float cardW, float cardH, float gap) {
        cards.clear();

        // pair IDs: 0..5 twice each => 12 cards
        vector<int> ids;
        for (int i = 0; i < (cols * rows) / 2; i++) {
            ids.push_back(i);
            ids.push_back(i);
        }
        random_shuffle(ids.begin(), ids.end());

        int k = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                float x = left + c * (cardW + gap);
                float y = bottom + (rows - 1 - r) * (cardH + gap);
                cards.push_back(Card(x, y, cardW, cardH, ids[k++]));
            }
        }
    }

    int indexAt(float px, float py) const {
        for (int i = 0; i < (int)cards.size(); i++) {
            if (cards[i].contains(px, py)) return i;
        }
        return -1;
    }

    bool allMatched() const {
        for (int i = 0; i < (int)cards.size(); i++) {
            if (!cards[i].matched) return false;
        }
        return true;
    }
};

class Game {
public:
    Board board;

    int moves;
    int matches;
    int timeLeft;

    bool gameOver;
    bool win;

    int first;
    int second;
    bool lockInput;

    int hoverIndex;

    Game() { reset(); }

    void reset() {
        moves = 0;
        matches = 0;
        timeLeft = 30;

        gameOver = false;
        win = false;

        first = -1;
        second = -1;
        lockInput = false;

        hoverIndex = -1;

        // Layout in OpenGL 2D (-1..1)
        float cardW = 0.36f;
        float cardH = 0.36f;
        float gap   = 0.06f;

        float totalW = board.cols * cardW + (board.cols - 1) * gap;
        float left   = -totalW / 2.0f;
        float bottom = -0.80f;

        board.create(left, bottom, cardW, cardH, gap);

        // Reset animation state
        for (int i = 0; i < (int)board.cards.size(); i++) {
            board.cards[i].matched = false;
            board.cards[i].setFaceUp(false);
            board.cards[i].flip = 0.0f;
            board.cards[i].targetFlip = 0.0f;
        }
    }

    void updateAnimation() {
        for (int i = 0; i < (int)board.cards.size(); i++) {
            board.cards[i].updateAnim();
        }
    }

    void setHover(float fx, float fy) {
        hoverIndex = board.indexAt(fx, fy);
    }

    void clickCard(int idx) {
        if (gameOver || lockInput) return;
        if (idx < 0 || idx >= (int)board.cards.size()) return;

        Card &c = board.cards[idx];
        if (c.matched || c.faceUp) return;

        c.setFaceUp(true);

        if (first == -1) {
            first = idx;
        } else if (second == -1) {
            second = idx;
            moves++;

            // Match?
            if (board.cards[first].pairID == board.cards[second].pairID) {
                board.cards[first].matched = true;
                board.cards[second].matched = true;
                matches++;

                first = -1;
                second = -1;

                if (board.allMatched()) {
                    gameOver = true;
                    win = true;
                }
            } else {
                lockInput = true;
                glutTimerFunc(700, Game::flipBackTimer, 0);
            }
        }
    }

    void flipBack() {
        if (first != -1 && second != -1) {
            board.cards[first].setFaceUp(false);
            board.cards[second].setFaceUp(false);
        }
        first = -1;
        second = -1;
        lockInput = false;
    }

    void tickOneSecond() {
        if (gameOver) return;

        timeLeft--;
        if (timeLeft <= 0) {
            timeLeft = 0;
            gameOver = true;
            win = board.allMatched();
        }
    }

    void draw() const {
        drawBackground();
        drawCards();
        drawUI();
    }

    static Game* instance() {
        static Game g;
        return &g;
    }

    static void flipBackTimer(int) {
        instance()->flipBack();
    }

private:
    void drawBackground() const {
        glBegin(GL_QUADS);
        glColor3f(0.08f, 0.10f, 0.16f); glVertex2f(-1,-1);
        glColor3f(0.08f, 0.10f, 0.16f); glVertex2f( 1,-1);
        glColor3f(0.14f, 0.18f, 0.28f); glVertex2f( 1, 1);
        glColor3f(0.14f, 0.18f, 0.28f); glVertex2f(-1, 1);
        glEnd();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0,0,0,0.35f);
        drawRect(-1.0f, 0.72f, 2.0f, 0.28f);
        glDisable(GL_BLEND);
    }

    void drawCards() const {
        for (int i = 0; i < (int)board.cards.size(); i++) {
            bool hover = (i == hoverIndex) && !gameOver && !lockInput;
            board.cards[i].draw(hover);
        }
    }

    void drawUI() const {
        glColor3f(1,1,1);

        // timer top center
        drawText(-0.02f, 0.86f, to_string(timeLeft), GLUT_BITMAP_TIMES_ROMAN_24);

        // Stats left
        drawText(-0.95f, 0.90f, "Moves: " + to_string(moves), GLUT_BITMAP_HELVETICA_18);
        drawText(-0.95f, 0.82f, "Matches: " + to_string(matches) + "/6", GLUT_BITMAP_HELVETICA_18);

        // Help bottom
        glColor3f(0.85f,0.85f,0.85f);
        drawText(-0.95f, -0.97f, "Click cards | R = Restart | ESC = Exit", GLUT_BITMAP_HELVETICA_12);

        if (gameOver) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0,0,0,0.60f);
            drawRect(-1.0f, -0.18f, 2.0f, 0.36f);
            glDisable(GL_BLEND);

            if (win) glColor3f(1.0f, 0.92f, 0.25f);
            else     glColor3f(1.0f, 0.40f, 0.40f);

            drawText(-0.13f, 0.02f, win ? "YOU WIN!" : "TIME UP!", GLUT_BITMAP_TIMES_ROMAN_24);

            glColor3f(1,1,1);
            drawText(-0.20f, -0.10f, "Press R to play again", GLUT_BITMAP_HELVETICA_18);
        }
    }
};


int winW = 900, winH = 900;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Game::instance()->draw();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    winW = w; winH = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int, int) {
    if (key == 27) exit(0);
    if (key == 'r' || key == 'R') {
        Game::instance()->reset();
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;


    float fx = (2.0f * x) / winW - 1.0f;
    float fy = 1.0f - (2.0f * y) / winH;

    int idx = Game::instance()->board.indexAt(fx, fy);
    Game::instance()->clickCard(idx);
    glutPostRedisplay();
}

void passiveMotion(int x, int y) {
    float fx = (2.0f * x) / winW - 1.0f;
    float fy = 1.0f - (2.0f * y) / winH;
    Game::instance()->setHover(fx, fy);
}

void animTimer(int) {
    Game::instance()->updateAnimation();
    glutPostRedisplay();
    glutTimerFunc(16, animTimer, 0);
}

void secondTimer(int) {
    Game::instance()->tickOneSecond();
    glutPostRedisplay();
    glutTimerFunc(1000, secondTimer, 0);
}

void initGL() {
    glClearColor(0,0,0,1);
    glDisable(GL_DEPTH_TEST);
}


int main(int argc, char** argv) {
    srand((unsigned)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(winW, winH);
    glutCreateWindow("Memory Card Game");

    initGL();
    Game::instance()->reset();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);

    glutTimerFunc(16, animTimer, 0);
    glutTimerFunc(1000, secondTimer, 0);

    glutMainLoop();
    return 0;
}

