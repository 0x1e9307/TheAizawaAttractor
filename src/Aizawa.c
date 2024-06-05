#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Constants for window size and title
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;
const char* WINDOW_TITLE = "Aizawa Attractor";

// Other constants
const double DEFAULT_POINT_SIZE = 1.0;
const int MAX_POINTS = 100000;
const int MIN_POINTS = 500;
const double MAX_POINT_SIZE = 3.0;
const double MIN_POINT_SIZE = 0.2;
const double DEFAULT_ANGLE_Z_INCREMENT = 1.0;

// Attractor parameters
double a = 0.95, b = 0.7, c = 0.6, d = 3.5, e = 0.25, f = 0.1;
double x = 0.1, y = 0, z = 0;

// Other global variables
double angleZ = 0.0;
int numPoints = 30000;
float pointSize = DEFAULT_POINT_SIZE;
int isFullScreen = 0;
float color[3] = {0.0, 1.0, 1.0};
int screenWidth = 1920;
int screenHeight = 1080;
char infoString[50];
int showInfo = 1;

// Function prototypes
void rk4_step(double *x, double *y, double *z, double dt);
void renderBitmapString(float x, float y, void *font, const char *string);
void display();
void idle();
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void setup();

// Function for numerical integration using the 4th order Runge-Kutta method
void rk4_step(double *x, double *y, double *z, double dt) {
    double dx1, dy1, dz1, dx2, dy2, dz2, dx3, dy3, dz3, dx4, dy4, dz4;
    double x_temp, y_temp, z_temp;

    dx1 = (*z - b) * (*x) - d * (*y);
    dy1 = d * (*x) + (*z - b) * (*y);
    dz1 = c + a * (*z) - pow(*z, 3) / 3 - (pow(*x, 2) + pow(*y, 2)) * (1 + e * (*z)) + f * (*z) * pow(*x, 3);

    x_temp = *x + 0.5 * dt * dx1;
    y_temp = *y + 0.5 * dt * dy1;
    z_temp = *z + 0.5 * dt * dz1;
    dx2 = (z_temp - b) * x_temp - d * y_temp;
    dy2 = d * x_temp + (z_temp - b) * y_temp;
    dz2 = c + a * z_temp - pow(z_temp, 3) / 3 - (pow(x_temp, 2) + pow(y_temp, 2)) * (1 + e * z_temp) + f * z_temp * pow(x_temp, 3);

    x_temp = *x + 0.5 * dt * dx2;
    y_temp = *y + 0.5 * dt * dy2;
    z_temp = *z + 0.5 * dt * dz2;
    dx3 = (z_temp - b) * x_temp - d * y_temp;
    dy3 = d * x_temp + (z_temp - b) * y_temp;
    dz3 = c + a * z_temp - pow(z_temp, 3) / 3 - (pow(x_temp, 2) + pow(y_temp, 2)) * (1 + e * z_temp) + f * z_temp * pow(x_temp, 3);

    x_temp = *x + dt * dx3;
    y_temp = *y + dt * dy3;
    z_temp = *z + dt * dz3;
    dx4 = (z_temp - b) * x_temp - d * y_temp;
    dy4 = d * x_temp + (z_temp - b) * y_temp;
    dz4 = c + a * z_temp - pow(z_temp, 3) / 3 - (pow(x_temp, 2) + pow(y_temp, 2)) * (1 + e * z_temp) + f * z_temp * pow(x_temp, 3);

    *x += (dt / 6.0) * (dx1 + 2 * dx2 + 2 * dx3 + dx4);
    *y += (dt / 6.0) * (dy1 + 2 * dy2 + 2 * dy3 + dy4);
    *z += (dt / 6.0) * (dz1 + 2 * dz2 + 2 * dz3 + dz4);
}

// Function to display information
void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Function to display the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);
    glColor3fv(color);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (int i = 0;  i < numPoints; i++) {
        rk4_step(&x, &y, &z, 0.1);
        glVertex3f(x, y, z);
    }
    glEnd();
    glPopMatrix();
    if (showInfo) {
        glColor3f(1.0f, 1.0f, 1.0f);
        renderBitmapString(-3.5f, 1.8f, GLUT_BITMAP_HELVETICA_12, infoString);
    }
    glutSwapBuffers();
}

// Function to handle animation
void idle() {
    angleZ += DEFAULT_ANGLE_Z_INCREMENT;
    if (angleZ >= 360.0) angleZ -= 360.0;
    glutPostRedisplay();
}

// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ASCII code for the Esc key
        case 'q':
            if (isFullScreen) {
                glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
                isFullScreen = 0;
            } else {
                exit(0);
            }
            break;
        case 'f':
            if (isFullScreen) {
                glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
                isFullScreen = 0;
            } else {
                glutFullScreen();
                isFullScreen = 1;
            }
            break;
        case 'r': // Enable random color
            color[0] = (float)rand() / RAND_MAX;
            color[1] = (float)rand() / RAND_MAX;
            color[2] = (float)rand() / RAND_MAX;
            break;
        case 'R': // Reset color to initial state
            color[0] = 0.0;
            color[1] = 1.0;
            color[2] = 1.0;
            break;
        case 'i': // Toggle information display
            showInfo = !showInfo;
            break;
    }
}

// Function to handle special keys
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: // Increase the number of points
            if (numPoints < MAX_POINTS)
                numPoints += 500;
            break;
        case GLUT_KEY_DOWN: // Decrease the number of points
            if (numPoints > MIN_POINTS)
                numPoints -= 500;
            break;
        case GLUT_KEY_RIGHT: // Increase point size
            if (pointSize < MAX_POINT_SIZE)
                pointSize += 0.1;
            break;
        case GLUT_KEY_LEFT: // Decrease point size
            if (pointSize > MIN_POINT_SIZE)
                pointSize -= 0.1;
            break;
    }
    sprintf(infoString, "Points: %d, Size: %.1f", numPoints, pointSize);
}

// Function to set up the scene
void setup() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPointSize(pointSize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(24.0, (double) WINDOW_WIDTH / (double) WINDOW_HEIGHT, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    sprintf(infoString, "Points: %d, Size: %.1f", numPoints, pointSize);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutPostRedisplay();

    setup();

    glutMainLoop();

    return 0;
}
