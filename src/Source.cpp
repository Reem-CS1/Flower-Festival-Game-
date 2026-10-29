/* * Project: Flower Festival
 * Major: Computer Science
 * Course: Computer Graphics (CS2206)
 * Group Members: Reem Alzahrani, Ritaj Ayman, Shaimaa Alshareef, Lana Alsalmi
 */

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <GL/glut.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freeglut.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <iostream>
using namespace std;


// ============================================================
//  Color palette
// ============================================================
const float SAGE_R = 0.53f, SAGE_G = 0.62f, SAGE_B = 0.52f;
const float CREAM_R = 0.81f, CREAM_G = 0.86f, CREAM_B = 0.85f;
const float DG_R = 0.12f, DG_G = 0.32f, DG_B = 0.18f;
const float PINK_R = 1.00f, PINK_G = 0.75f, PINK_B = 0.80f;
const float PI = 3.14f;

// ============================================================
//  Global state
// ============================================================

int    scene = 0;
int    selectedVase = 0;
int    selectedFlowers = 0;
bool   isHover = false;
float  timeValue = 0.0f;

float  dragX = -0.82f;
float  dragY = 0.55f;
bool   isDragging = false;
int    dragFlowerType = 1;
int    petalsPlaced = 0;

float  pulseScale = 1.0f;
bool   pulseUp = true;
float  orbitAngle = 0.0f;

// ============================================================
//    texture
// ============================================================

// buffers
GLuint myTexture1;
GLuint myTexture2;
    // Place grass.bmp (300x225) and petal.bmp (256x256) next to the .exe
char image1Path[] = R"(C:\Users\reemf\Downloads\grass1.bmp)";
char image2Path[] = R"(C:\Users\reemf\Downloads\Pink Square1.bmp)";


// ============================================================
//  Sound  (Extra — Windows Beep)
//  Reference: Microsoft Corporation. (n.d.). Beep function (utilapiset.h).
// https://learn.microsoft.com/en-us/windows/win32/api/utilapiset/nf-utilapiset-beep
// ============================================================
void playSound(int freq, int dur) {
#ifdef _WIN32
    Beep(freq, dur);
#endif
}

// ============================================================
//  Drawing utilities
//  Text Reference: Kilgard, M. J. (1996). The OpenGL Utility Toolkit (GLUT) API Version 3.
//  https://www.opengl.org/resources/libraries/glut/spec3/node76.html
// ============================================================
void drawString(float x, float y, const string& s, void* font,
    float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : s) glutBitmapCharacter(font, c);
}

void drawCircle(float cx, float cy, float radius,
    float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 360; i++) {
        float a = i * PI / 180.0f;
        glVertex2f(cx + radius * cosf(a), cy + radius * sinf(a));
    }
    glEnd();
}

void drawMiniFlower(float x, float y, float size,
    float r, float g, float b) {
    for (int i = 0; i < 5; i++) {
        float a = i * 72.0f * PI / 180.0f;
        drawCircle(x + size * 0.6f * cosf(a), y + size * 0.6f * sinf(a),
            size * 0.5f, r, g, b);
    }
    drawCircle(x, y, size * 0.3f, 1.0f, 0.85f, 0.2f);
}

void drawRose(float x, float y, float size) {
    for (int i = 0; i < 6; i++) {
        float a = i * 60.0f * PI / 180.0f;
        drawCircle(x + size * 0.55f * cosf(a), y + size * 0.55f * sinf(a),
            size * 0.45f, 0.9f, 0.15f, 0.25f);
    }
    for (int i = 0; i < 4; i++) {
        float a = i * 90.0f * PI / 180.0f + 0.4f;
        drawCircle(x + size * 0.28f * cosf(a), y + size * 0.28f * sinf(a),
            size * 0.28f, 0.75f, 0.05f, 0.15f);
    }
    drawCircle(x, y, size * 0.18f, 0.6f, 0.02f, 0.08f);
}

void drawTulip(float x, float y, float size) {
    glColor3f(0.15f, 0.45f, 0.15f); glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y - (size * 0.6f));
    glVertex2f(x, y);
    glEnd();


    glColor3f(0.95f, 0.35f, 0.65f);
    glBegin(GL_POLYGON);
    glVertex2f(x - size * 0.35f, y);
    glVertex2f(x + size * 0.35f, y);
    glVertex2f(x + size * 0.25f, y + size * 0.7f);
    glVertex2f(x - size * 0.25f, y + size * 0.7f);
    glEnd();


    drawCircle(x, y + size * 0.7f, size * 0.27f, 0.95f, 0.35f, 0.65f);


    glColor3f(1.0f, 0.65f, 0.80f);
    glBegin(GL_POLYGON);
    glVertex2f(x - size * 0.12f, y + size * 0.10f);
    glVertex2f(x, y + size * 0.10f);
    glVertex2f(x, y + size * 0.65f);
    glVertex2f(x - size * 0.08f, y + size * 0.65f);
    glEnd();
}

void drawDaisy(float x, float y, float size) {
    for (int i = 0; i < 10; i++) {
        float a = i * 36.0f * PI / 180.0f;
        drawCircle(x + size * 0.6f * cosf(a), y + size * 0.6f * sinf(a),
            size * 0.35f, 1.0f, 1.0f, 0.85f);
    }
    drawCircle(x, y, size * 0.3f, 1.0f, 0.85f, 0.1f);
}

void drawStem(float x, float y, float h) {
    glColor3f(0.15f, 0.45f, 0.15f); glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y - h); glVertex2f(x, y);
    glEnd();
    glColor3f(0.2f, 0.5f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y - h * 0.40f);
    glVertex2f(x + h * 0.18f, y - h * 0.25f);
    glVertex2f(x + h * 0.22f, y - h * 0.15f);
    glVertex2f(x, y - h * 0.20f);
    glEnd();
}

// ============================================================
//  Vase drawing 
// ============================================================
void drawVase1(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(1.5f, 1.5f, 1.0f);

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 40; i++) {
        float t = (float)i / 40.0f;
        float yp = -0.10f + t * 0.22f;
        float w = 0.04f + 0.04f * sinf(t * PI);
        glColor3f(0.6f + t * 0.2f, 0.5f + t * 0.2f, 0.4f + t * 0.2f);
        glVertex2f(-w, yp); glVertex2f(w, yp);
    }
    glEnd();

    glColor3f(0.7f, 0.6f, 0.5f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.03f, 0.12f); glVertex2f(-0.035f, 0.15f);
    glVertex2f(0.035f, 0.15f); glVertex2f(0.03f, 0.12f);
    glEnd();

    glColor4f(1, 1, 1, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.01f, -0.05f); glVertex2f(0.0f, -0.05f);
    glVertex2f(0.015f, 0.05f); glVertex2f(0.0f, 0.08f);
    glVertex2f(-0.01f, 0.05f);
    glEnd();

    if (selectedVase == 1) {
        glColor3f(1, 1, 1); glLineWidth(3);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.10f, -0.15f); glVertex2f(0.10f, -0.15f);
        glVertex2f(0.10f, 0.20f); glVertex2f(-0.10f, 0.20f);
        glEnd();
    }
    glPopMatrix();
}

void drawSmallFlower(float cx, float cy) {
    glColor3f(1.0f, 0.3f, 0.5f);
    for (int i = 0; i < 6; i++) {
        float a = i * 2.0f * PI / 6;
        float px = cx + 0.015f * cosf(a), py = cy + 0.015f * sinf(a);
        glBegin(GL_POLYGON);
        for (int j = 0; j < 20; j++) {
            float b = 2.0f * PI * j / 20;
            glVertex2f(px + 0.008f * cosf(b), py + 0.008f * sinf(b));
        }
        glEnd();
    }
    glColor3f(1, 1, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = 2.0f * PI * i / 20;
        glVertex2f(cx + 0.006f * cosf(a), cy + 0.006f * sinf(a));
    }
    glEnd();
}

void drawVase2(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(1.5f, 1.5f, 1.0f);

    glColor3f(1.0f, 0.6f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, -0.10f); glVertex2f(-0.08f, 0.00f);
    glVertex2f(-0.06f, 0.08f); glVertex2f(-0.03f, 0.11f);
    glVertex2f(0.03f, 0.11f); glVertex2f(0.06f, 0.08f);
    glVertex2f(0.08f, 0.00f); glVertex2f(0.05f, -0.10f);
    glEnd();

    glColor3f(0.9f, 0.4f, 0.5f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, 0.10f); glVertex2f(-0.06f, 0.13f);
    glVertex2f(0.06f, 0.13f); glVertex2f(0.05f, 0.10f);
    glEnd();

    if (selectedVase == 2) {
        glColor3f(1, 1, 1); glLineWidth(3);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.10f, -0.15f); glVertex2f(0.10f, -0.15f);
        glVertex2f(0.10f, 0.20f); glVertex2f(-0.10f, 0.20f);
        glEnd();
    }
    drawSmallFlower(-0.03f, 0.02f);
    drawSmallFlower(0.03f, 0.02f);
    drawSmallFlower(0.00f, -0.03f);
    glPopMatrix();
}

void drawVase3(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(1.5f, 1.5f, 1.0f);

    glColor3f(0.85f, 0.50f, 0.30f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 50; i++) {
        float a = 2 * PI * i / 50;
        glVertex2f(cosf(a) * 0.11f, sinf(a) * 0.10f);
    }
    glEnd();

    glColor3f(0.95f, 0.75f, 0.60f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 50; i++) {
        float a = 2 * PI * i / 50;
        glVertex2f(cosf(a) * 0.07f, 0.08f + sinf(a) * 0.03f);
    }
    glEnd();

    glColor4f(1, 1, 1, 0.3f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 50; i++) {
        float a = 2 * PI * i / 50;
        glVertex2f(-0.03f + cosf(a) * 0.03f, 0.02f + sinf(a) * 0.05f);
    }
    glEnd();

    if (selectedVase == 3) {
        glLineWidth(2.5f); glColor3f(1, 1, 1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.14f, -0.11f); glVertex2f(0.14f, -0.11f);
        glVertex2f(0.14f, 0.13f); glVertex2f(-0.14f, 0.13f);
        glEnd();
    }
    glPopMatrix();
}

// ============================================================
//  bouquet decoration
// ============================================================
void drawOriginalBouquet(float x, float y) {
    glColor3f(0.15f, 0.3f, 0.15f); glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(x, y - 0.10f); glVertex2f(x - 0.06f, y + 0.08f);
    glVertex2f(x, y - 0.10f); glVertex2f(x, y + 0.10f);
    glVertex2f(x, y - 0.10f); glVertex2f(x + 0.06f, y + 0.08f);
    glEnd();
    drawMiniFlower(x, y + 0.12f, 0.055f, 0.9f, 0.2f, 0.2f);
    drawMiniFlower(x - 0.08f, y + 0.06f, 0.045f, 1.0f, 0.5f, 0.6f);
    drawMiniFlower(x + 0.08f, y + 0.06f, 0.045f, 1.0f, 0.7f, 0.3f);
    drawMiniFlower(x, y + 0.03f, 0.040f, 1.0f, 0.9f, 0.4f);
}

// ============================================================
//  UI elements
// ============================================================
void drawSparkles() {
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 30; i++) {
        float x = -0.9f + i * 0.07f, y = 0.85f - (i % 5) * 0.28f;
        float glow = (sinf(timeValue) + 1) * 0.5f;
        glColor3f(1, 1, glow);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawCreamFrame() {
    glColor3f(CREAM_R, CREAM_G, CREAM_B);
    glBegin(GL_QUADS);
    glVertex2f(-0.68f, 0.40f); glVertex2f(0.68f, 0.40f);
    glVertex2f(0.68f, -0.40f); glVertex2f(-0.68f, -0.40f);
    glEnd();
    glColor3f(1, 1, 1); glLineWidth(2.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.68f, 0.40f); glVertex2f(0.68f, 0.40f);
    glVertex2f(0.68f, -0.40f); glVertex2f(-0.68f, -0.40f);
    glEnd();
}

GLuint LoadTexture(const char* filename)
{
    GLuint texture;
    FILE* file = fopen(filename, "rb");

    if (!file)
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }

    unsigned char header[54];
    fread(header, 1, 54, file); 

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int imageSize = width * height * 3;

    unsigned char* data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    for (int i = 0; i < imageSize; i += 3)
    {
        swap(data[i], data[i + 2]);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, data);

    delete[] data;
    return texture;
}

// Textured ground (Texture 1 — grass)
void drawTexturedGround() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(4, 0); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(4, 2); glVertex2f(1.0f, -0.62f);
    glTexCoord2f(0, 2); glVertex2f(-1.0f, -0.62f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Textured panel behind vase (Texture 2 — petal)
void drawTexturedPanel(float x, float y, float w, float h) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture2);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor4f(1, 1, 1, 0.50f);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(1, 0); glVertex2f(x + w, y);
    glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
    glTexCoord2f(0, 1); glVertex2f(x, y + h);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Helper — draw the selected flower at a position
void drawFlowerByType(int type, float x, float y, float size) {
    if (type == 1)      drawRose(x, y, size);
    else if (type == 2) drawTulip(x, y, size * 1.5f);
    else              drawDaisy(x, y, size);
}

// ============================================================
//  SCENE 0 —  Title
// ============================================================
void displayScene0() {
    drawSparkles();

    // Title text  (Extra: text)
    drawString(-0.32f, 0.30f, "Annual Flower Festival",
        GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);

    glColor3f(1, 1, 1); glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(-0.50f, 0.20f); glVertex2f(0.50f, 0.20f);
    glEnd();

    drawString(-0.30f, 0.10f,
        "Design a bouquet full of colors and joy",
        GLUT_BITMAP_HELVETICA_12, 1, 1, 1);

    // Animated bouquet 
    float off = sinf(timeValue) * 0.012f;
    glPushMatrix();
    glTranslatef(off, 0.0f, 0.0f);    // Translation
    drawOriginalBouquet(0.0f, -0.15f); 
    glPopMatrix();

    // Button shadow
    glColor3f(0.3f, 0.4f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(-0.38f, -0.42f); glVertex2f(0.42f, -0.42f);
    glVertex2f(0.42f, -0.64f); glVertex2f(-0.38f, -0.64f);
    glEnd();

    if (isHover) glColor3f(PINK_R, PINK_G, PINK_B);
    else         glColor3f(CREAM_R, CREAM_G, CREAM_B);
    glBegin(GL_QUADS);
    glVertex2f(-0.395f, -0.415f); glVertex2f(0.395f, -0.415f);
    glVertex2f(0.395f, -0.630f); glVertex2f(-0.395f, -0.630f);
    glEnd();

    drawString(-0.29f, -0.54f,
        "Create your first beautiful bouquet !!",
        GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);
}

// ============================================================
//  SCENE 1 — Story introduction
// ============================================================
void displayScene1() {
    drawCreamFrame();
    drawString(-0.60f, 0.24f, "The Annual Flower Festival is near!",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawString(-0.60f, 0.08f, "Every year, the most beautiful bouquet",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawString(-0.60f, -0.06f, "wins the Grand Floral Prize.",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawString(-0.60f, -0.22f, "This year, it is YOUR turn !",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawOriginalBouquet(0.52f, 0.05f);
    drawString(-0.25f, -0.55f, "Press ENTER to continue",
        GLUT_BITMAP_HELVETICA_12, 1, 1, 1);
}

// ============================================================
//  SCENE 2 — Instructions
// ============================================================
void displayScene2() {
    drawCreamFrame();
    drawString(-0.58f, 0.28f, "How to Play:",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawString(-0.58f, 0.14f, "1.  Press ENTER to move between scenes",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawString(-0.58f, 0.01f, "2.  Click a vase to select it",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawString(-0.58f, -0.12f, "3.  Toggle flowers by clicking them",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawString(-0.58f, -0.25f, "4.  Drag flowers into the yellow box",
        GLUT_BITMAP_HELVETICA_18, DG_R, DG_G, DG_B);
    drawOriginalBouquet(0.52f, 0.05f);
    drawString(-0.25f, -0.55f, "Click anywhere to continue",
        GLUT_BITMAP_HELVETICA_12, 1, 1, 1);
}

// ============================================================
//  SCENE 3 — Vase selection
// ============================================================
void displayScene3() {
    drawCreamFrame();
    drawString(-0.28f, 0.30f, "Choose Your Vase",
        GLUT_BITMAP_TIMES_ROMAN_24, DG_R, DG_G, DG_B);

    drawVase1(-0.40f, -0.05f);
    drawVase2(0.00f, -0.05f);
    drawVase3(0.40f, -0.05f);

    // Fixed Text Positioning
    drawString(-0.43f, -0.35f, "Tall", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);
    drawString(-0.06f, -0.35f, "Classic", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);
    drawString(0.35f, -0.35f, "Round", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);
    drawString(-0.25f, -0.55f, "Click to choose, then press ENTER",
        GLUT_BITMAP_HELVETICA_12, 1, 1, 1);
}

// ============================================================
//  SCENE 4 — Flower selection
// ============================================================
void displayScene4() {
    drawCreamFrame();
    drawString(-0.30f, 0.30f, "Choose Your Flowers",
        GLUT_BITMAP_TIMES_ROMAN_24, DG_R, DG_G, DG_B);

    // Rose
    drawStem(-0.42f, -0.05f, 0.18f);
    drawRose(-0.42f, -0.05f, 0.10f);
    if (selectedFlowers & 1) {
        glColor3f(PINK_R, PINK_G, PINK_B); glLineWidth(2.5f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.56f, -0.26f); glVertex2f(-0.28f, -0.26f);
        glVertex2f(-0.28f, 0.08f); glVertex2f(-0.56f, 0.08f);
        glEnd();
    }
    drawString(-0.46f, -0.32f, "Rose", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);

    // Tulip
    drawTulip(0.0f, -0.15f, 0.18f);
    if (selectedFlowers & 2) {
        glColor3f(PINK_R, PINK_G, PINK_B); glLineWidth(2.5f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.12f, -0.26f); glVertex2f(0.12f, -0.26f);
        glVertex2f(0.12f, 0.08f);  glVertex2f(-0.12f, 0.08f);
        glEnd();
    }
    drawString(-0.04f, -0.32f, "Tulip", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);

    // Daisy
    drawStem(0.42f, -0.05f, 0.18f);
    drawDaisy(0.42f, -0.05f, 0.10f);
    if (selectedFlowers & 4) {
        glColor3f(PINK_R, PINK_G, PINK_B); glLineWidth(2.5f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0.28f, -0.26f); glVertex2f(0.56f, -0.26f);
        glVertex2f(0.56f, 0.08f); glVertex2f(0.28f, 0.08f);
        glEnd();
    }
    drawString(0.38f, -0.32f, "Daisy", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);

    drawString(-0.25f, -0.55f, "Click flowers, then press ENTER",
        GLUT_BITMAP_HELVETICA_12, 1, 1, 1);
}

// ============================================================
//  SCENE 5 — Arrangement  (textures + drag + transformations)
// ============================================================
void displayScene5() {
    // Texture 1: grass ground
    drawTexturedGround();

    // Texture 2: petal panel behind vase
    drawTexturedPanel(-0.32f, -0.62f, 0.64f, 0.50f);

    drawString(-0.24f, 0.50f, "Arrange your bouquet!",
        GLUT_BITMAP_HELVETICA_18, 0.1f, 0.2f, 0.1f);

    // Selected vase — Fixed Scale and Position
    int sv = selectedVase; selectedVase = 0;
    glPushMatrix();
    glTranslatef(0.0f, -0.18f, 0.0f);
    glScalef(1.7f, 1.7f, 1.0f);           
    if (sv == 1)      drawVase1(0.0f, 0.0f);
    else if (sv == 2) drawVase2(0.0f, 0.0f);
    else            drawVase3(0.0f, 0.0f);
    glPopMatrix();
    selectedVase = sv;

    // Placed flowers inside vase
    for (int i = 0; i < petalsPlaced && i < 6; i++) {
        float fx = -0.15f + i * 0.06f;
        float fy = 0.12f + (i % 2) * 0.07f;
        drawFlowerByType(dragFlowerType, fx, fy, 0.065f);
    }

    // Yellow drop zone box
    glColor3f(1.0f, 1.0f, 0.4f); glLineWidth(2.0f);
    glEnable(GL_LINE_STIPPLE); glLineStipple(2, 0xAAAA);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.28f, -0.58f); glVertex2f(0.28f, -0.58f);
    glVertex2f(0.28f, 0.20f); glVertex2f(-0.28f, 0.20f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    // Flower shelf (left)
    drawString(-0.96f, 0.82f, "Flowers:", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);
    if (selectedFlowers & 1) {
        drawStem(-0.82f, 0.58f, 0.14f);
        drawRose(-0.82f, 0.58f, 0.07f);
        drawString(-0.92f, 0.38f, "Rose", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);
    }
    if (selectedFlowers & 2) {
        drawTulip(-0.82f, 0.18f, 0.14f);
        drawString(-0.92f, -0.02f, "Tulip", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);
    }
    if (selectedFlowers & 4) {
        drawStem(-0.82f, -0.20f, 0.14f);
        drawDaisy(-0.82f, -0.20f, 0.07f);
        drawString(-0.92f, -0.40f, "Daisy", GLUT_BITMAP_HELVETICA_12, DG_R, DG_G, DG_B);
    }

    // Dragged flower 
    if (isDragging) {
        glPushMatrix();
        glTranslatef(dragX, dragY, 0.0f);               // Translation
        float sc = 1.0f + 0.06f * sinf(timeValue * 2.0f);
        glScalef(sc, sc, 1.0f);                         // Scale
        drawFlowerByType(dragFlowerType, 0, 0, 0.08f);
        glPopMatrix();
    }

    drawString(-0.43f, -0.94f,
        "Drag flowers into the yellow box, then press ENTER",
        GLUT_BITMAP_HELVETICA_12, 1, 1, 1);
}
// ============================================================
//  SCENE 6 — Result  (Rotation + Scaling transformations)
// ============================================================
void displayScene6() {
    drawSparkles();

    drawString(-0.32f, 0.90f, "Your Bouquet is Ready!",
        GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 0.7f);

    // Vase —  (Scaling transformation)
    int sv = selectedVase; selectedVase = 0;
    glPushMatrix();
    glTranslatef(0.0f, -0.18f, 0.0f);
    glScalef(pulseScale * 1.8f, pulseScale * 1.8f, 1.0f);   // Scale
    if (sv == 1)      drawVase1(0.0f, 0.0f);
    else if (sv == 2) drawVase2(0.0f, 0.0f);
    else            drawVase3(0.0f, 0.0f);
    glPopMatrix();
    selectedVase = sv;

    // Placed flowers shown in vase
    for (int i = 0; i < petalsPlaced && i < 6; i++) {
        float fx = -0.15f + i * 0.06f;
        float fy = 0.12f + (i % 2) * 0.07f;
        drawFlowerByType(dragFlowerType, fx, fy, 0.065f);
    }

    // Orbiting mini-flowers — Rotation + Translation + Scaling
    for (int i = 0; i < 6; i++) {
        float angle = i * 60.0f * PI / 180.0f + orbitAngle;
        float radius = 0.56f + 0.04f * sinf(timeValue + i);
        float fx = radius * cosf(angle);
        float fy = radius * sinf(angle);
        float fs = 0.045f + 0.012f * sinf(timeValue + i);

        glPushMatrix();
        glTranslatef(fx, fy, 0.0f);                     // Translation
        glRotatef(orbitAngle * 180.0f / PI + i * 60.0f, 0, 0, 1);// Rotation
        glScalef(fs * 12, fs * 12, 1.0f);                   // Scale
        drawMiniFlower(0, 0, 0.1f,
            (i % 3 == 0) ? 0.9f : 1.0f,
            (i % 3 == 1) ? 0.8f : 0.4f,
            (i % 3 == 2) ? 0.8f : 0.3f);
        glPopMatrix();
    }
}


// ============================================================
//  Master display
// ============================================================
void display() {
    glClearColor(SAGE_R, SAGE_G, SAGE_B, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (scene == 0) displayScene0();
    else if (scene == 1) displayScene1();
    else if (scene == 2) displayScene2();
    else if (scene == 3) displayScene3();
    else if (scene == 4) displayScene4();
    else if (scene == 5) displayScene5();
    else if (scene == 6) displayScene6();

    glutSwapBuffers();
}

// ============================================================
//  Keyboard
// ============================================================
void keyboard(unsigned char key, int x, int y) {
    if (key == 13) {  
        if (scene == 1)                       scene = 2;
        else if (scene == 3 && selectedVase != 0)    scene = 4;
        else if (scene == 4)                       scene = 5;
        else if (scene == 5)                       scene = 6;
        else if (scene == 6)                       scene = 7;
        else if (scene == 7) {
            scene = 0; selectedVase = 0; selectedFlowers = 0;
            petalsPlaced = 0; orbitAngle = 0;
            dragX = -0.82f; dragY = 0.55f;
        }
        playSound(480 + scene * 70, 80);   // Sound
    }
    if (key == 27) exit(0);
    glutPostRedisplay();
}

// ============================================================
//  Mouse
// ============================================================
void mouse(int button, int state, int x, int y) {
    float ox = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 2.0f) - 1.0f;
    float oy = 1.0f - (float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 2.0f);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (scene == 0) {
            if (ox >= -0.4f && ox <= 0.4f && oy >= -0.63f && oy <= -0.41f) {
                scene = 1; playSound(440, 80);
            }
        }
        else if (scene == 2) { scene = 3; playSound(440, 80); }
        else if (scene == 3) {
            if (ox >= -0.55f && ox <= -0.28f && oy >= -0.22f && oy <= 0.12f) selectedVase = 1;
            if (ox >= -0.14f && ox <= 0.14f && oy >= -0.22f && oy <= 0.12f) selectedVase = 2;
            if (ox >= 0.26f && ox <= 0.56f && oy >= -0.22f && oy <= 0.12f) selectedVase = 3;
            playSound(380, 50);
        }
        else if (scene == 4) {
            if (ox >= -0.56f && ox <= -0.28f && oy >= -0.26f && oy <= 0.08f) { selectedFlowers ^= 1; playSound(350, 50); }
            if (ox >= -0.12f && ox <= 0.12f && oy >= -0.35f && oy <= 0.08f) { selectedFlowers ^= 2; playSound(350, 50); }
            if (ox >= 0.28f && ox <= 0.56f && oy >= -0.26f && oy <= 0.08f) { selectedFlowers ^= 4; playSound(350, 50); }
        }
        else if (scene == 5) {
            // Pick from shelf
            if (ox >= -0.96f && ox <= -0.68f) {
                if (oy >= 0.44f && oy <= 0.68f && (selectedFlowers & 1)) {
                    isDragging = true; dragFlowerType = 1; dragX = ox; dragY = oy;
                }
                else if (oy >= -0.02f && oy <= 0.28f && (selectedFlowers & 2)) {
                    isDragging = true; dragFlowerType = 2; dragX = ox; dragY = oy;
                }
                else if (oy >= -0.38f && oy <= -0.08f && (selectedFlowers & 4)) {
                    isDragging = true; dragFlowerType = 3; dragX = ox; dragY = oy;
                }
            }
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (scene == 5 && isDragging) {
            isDragging = false;
            // Drop inside yellow box
            if (dragX >= -0.28f && dragX <= 0.28f && dragY >= -0.58f && dragY <= 0.20f) {
                petalsPlaced++;
                playSound(660, 60);    // Sound 
            }
            dragX = -0.82f; dragY = 0.55f;
        }
    }
    glutPostRedisplay();
}

// Drag motion
void motionDrag(int x, int y) {
    float ox = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 2.0f) - 1.0f;
    float oy = 1.0f - (float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 2.0f);
    if (isDragging) { dragX = ox; dragY = oy; }
    glutPostRedisplay();
}

// Hover for button
void passiveMotion(int x, int y) {
    float ox = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 2.0f) - 1.0f;
    float oy = 1.0f - (float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 2.0f);
    isHover = (scene == 0 && ox >= -0.4f && ox <= 0.4f && oy >= -0.63f && oy <= -0.41f);
    glutPostRedisplay();
}



void loadTextures()
{
myTexture1 = LoadTexture(image1Path);
myTexture2 = LoadTexture(image2Path);
}

// ============================================================
//  Timer — animation
// ============================================================
void update(int) {
    timeValue += 0.05f;

    if (scene == 6) {
        orbitAngle += 0.012f;
        if (orbitAngle >= 2 * PI) orbitAngle -= 2 * PI;
        if (pulseUp) { pulseScale += 0.003f; if (pulseScale >= 1.08f) pulseUp = false; }
        else { pulseScale -= 0.003f; if (pulseScale <= 0.93f) pulseUp = true; }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// ============================================================
//  Init 
// ============================================================
void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    loadTextures();
}


// ============================================================
//  Main
// ============================================================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(750, 600);
    glutCreateWindow("Flower Festival - CS2206");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motionDrag);
    glutPassiveMotionFunc(passiveMotion);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
