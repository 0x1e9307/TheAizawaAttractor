#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Window parameters
int WINDOW_WIDTH = 960;
int WINDOW_HEIGHT = 540;
char* WINDOW_TITLE = "Aizawa Attractor";

// Rendering parameters
const double DEFAULT_POINT_SIZE = 1.0;
const int MAX_POINTS = 100000;
const int MIN_POINTS = 500;
const double MAX_POINT_SIZE = 3.0;
const double MIN_POINT_SIZE = 0.2;
double DEFAULT_ANGLE_Z_INCREMENT = 1.0;
const float INFO_MARGIN_LEFT = 40.0f;
const float INFO_MARGIN_TOP = 40.0f;

// Attractor parameters
double a_param = 0.95, b_param = 0.7, c_param = 0.6, d_param = 3.5, e_param = 0.25, f_param = 0.1;
double x_val = 0.1, y_val = 0, z_val = 0;

// Global variables
double angleZ = 0.0;
int numPoints = 30000;
float pointSize = DEFAULT_POINT_SIZE;
int isFullScreen = 0;
float color[3] = {0.0, 1.0, 1.0};
char infoString[50];
int showInfo = 1;

// Function prototypes
void integrate_rk4_step(double *x, double *y, double *z, double dt, double *dx, double *dy, double *dz);
void display();
void idle();
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void setup();
void reshape(int w, int h);

// Numerical integration using the 4th order Runge-Kutta method
void integrate_rk4_step(double *x, double *y, double *z, double dt, double *dx, double *dy, double *dz) {
    *dx = (*z - b_param) * (*x) - d_param * (*y);
    *dy = d_param * (*x) + (*z - b_param) * (*y);
    *dz = c_param + a_param * (*z) - pow(*z, 3) / 3 - ((*x) * (*x) + (*y) * (*y)) * (1 + e_param * (*z)) + f_param * (*z) * pow(*x, 3);
}

// Function to integrate using the 4th order Runge-Kutta method
void integrate_rk4(double *x, double *y, double *z, double dt) {
    double dx1, dy1, dz1, dx2, dy2, dz2, dx3, dy3, dz3, dx4, dy4, dz4;
    double x_temp, y_temp, z_temp;

    integrate_rk4_step(x, y, z, dt, &dx1, &dy1, &dz1);

    x_temp = *x + 0.5 * dt * dx1;
    y_temp = *y + 0.5 * dt * dy1;
    z_temp = *z + 0.5 * dt * dz1;
    integrate_rk4_step(&x_temp, &y_temp, &z_temp, dt, &dx2, &dy2, &dz2);

    x_temp = *x + 0.5 * dt * dx2;
    y_temp = *y + 0.5 * dt * dy2;
    z_temp = *z + 0.5 * dt * dz2;
    integrate_rk4_step(&x_temp, &y_temp, &z_temp, dt, &dx3, &dy3, &dz3);

    x_temp = *x + dt * dx3;
    y_temp = *y + dt * dy3;
    z_temp = *z + dt * dz3;
    integrate_rk4_step(&x_temp, &y_temp, &z_temp, dt, &dx4, &dy4, &dz4);

    *x += (dt / 6.0) * (dx1 + 2 * dx2 + 2 * dx3 + dx4);
    *y += (dt / 6.0) * (dy1 + 2 * dy2 + 2 * dy3 + dy4);
    *z += (dt / 6.0) * (dz1 + 2 * dz2 + 2 * dz3 + dz4);
}

// Function to display information
void display_info(float x, float y, void *font, const char *string) {
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
        integrate_rk4(&x_val, &y_val, &z_val, 0.1);
        glVertex3f(x_val, y_val, z_val);
    }
    glEnd();
    glPopMatrix();

    // Set text position at the top left of the screen
    if (showInfo) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glColor3f(1.0f, 1.0f, 1.0f);
        display_info(INFO_MARGIN_LEFT, INFO_MARGIN_TOP, GLUT_BITMAP_HELVETICA_12, infoString);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
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
        case 'r':
            if (DEFAULT_ANGLE_Z_INCREMENT)
                DEFAULT_ANGLE_Z_INCREMENT = 0.0f;
            else
                DEFAULT_ANGLE_Z_INCREMENT = 1.0f;
            break;
        case 'c':
            color[0] = (float)rand() / RAND_MAX;
            color[1] = (float)rand() / RAND_MAX;
            color[2] = (float)rand() / RAND_MAX;
            break;
        case 'C':
            color[0] = 0.0;
            color[1] = 1.0;
            color[2] = 1.0;
            break;
        case 'i':
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

// Function to handle window resizing
void reshape(int w, int h) {
    if (h == 0) h = 1; // Prevent division by zero
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(24.0, (double)w / (double)h, 1.0, 100.0);
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
    glutReshapeFunc(reshape);

    glutPostRedisplay();

    setup();

    glutMainLoop();

    return 0;
}
