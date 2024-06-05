#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Parameters of the Aizawa attractor
double a = 0.95, b = 0.7, c = 0.6, d = 3.5, e = 0.25, f = 0.1;
double x = 0.1, y = 0, z = 0;

// Rotation angle around the Z axis
double angleZ = 0.0;

// Number of points of the attractor
int numPoints = 30000;

// Point size
float pointSize = 1.0;

// Full-screen mode switch
int isFullScreen = 0;

// Attractor color
float color[3] = {0.0, 1.0, 1.0};

// Screen dimensions
int screenWidth = 1920;
int screenHeight = 1080;

// Global variable to track point size and number of points
char infoString[50];

// Flag to control information display
int showInfo = 1;

// Function for numerical integration using the 4th order Runge-Kutta method
void rk4_step(double *x, double *y, double *z, double dt) {
    // Declare variables for the Runge-Kutta method
    double dx1, dy1, dz1, dx2, dy2, dz2, dx3, dy3, dz3, dx4, dy4, dz4;
    double x_temp, y_temp, z_temp;

    // Calculate the derivatives at the current point using the Aizawa attractor equations
    dx1 = (*z - b) * (*x) - d * (*y);
    dy1 = d * (*x) + (*z - b) * (*y);
    dz1 = c + a * (*z) - pow(*z, 3) / 3 - (pow(*x, 2) + pow(*y, 2)) * (1 + e * (*z)) + f * (*z) * pow(*x, 3);

    // Calculate the derivatives at the midpoint of the time interval
    x_temp = *x + 0.5 * dt * dx1;
    y_temp = *y + 0.5 * dt * dy1;
    z_temp = *z + 0.5 * dt * dz1;
    dx2 = (z_temp - b) * x_temp - d * y_temp;
    dy2 = d * x_temp + (z_temp - b) * y_temp;
    dz2 = c + a * z_temp - pow(z_temp, 3) / 3 - (pow(x_temp, 2) + pow(y_temp, 2)) * (1 + e * z_temp) + f * z_temp * pow(x_temp, 3);

    // Calculate the derivatives at the midpoint of the time interval using the previous derivatives
    x_temp = *x + 0.5 * dt * dx2;
    y_temp = *y + 0.5 * dt * dy2;
    z_temp = *z + 0.5 * dt * dz2;
    dx3 = (z_temp - b) * x_temp - d * y_temp;
    dy3 = d * x_temp + (z_temp - b) * y_temp;
    dz3 = c + a * z_temp - pow(z_temp, 3) / 3 - (pow(x_temp, 2) + pow(y_temp, 2)) * (1 + e * z_temp) + f * z_temp * pow(x_temp, 3);

    // Calculate the derivatives at the end of the time interval
    x_temp = *x + dt * dx3;
    y_temp = *y + dt * dy3;
    z_temp = *z + dt * dz3;
    dx4 = (z_temp - b) * x_temp - d * y_temp;
    dy4 = d * x_temp + (z_temp - b) * y_temp;
    dz4 = c + a * z_temp - pow(z_temp, 3) / 3 - (pow(x_temp, 2) + pow(y_temp, 2)) * (1 + e * z_temp) + f * z_temp * pow(x_temp, 3);

    // Update the coordinates using the weighted sum of derivatives
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
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Push the current matrix onto the matrix stack
    glPushMatrix();

    // Rotate by 90 degrees around the X axis
    glRotatef(90, 1.0, 0.0, 0.0);

    // Rotate around the Z axis based on the angleZ variable
    glRotatef(angleZ, 0.0, 0.0, 1.0);

    // Set the current color
    glColor3fv(color);

    // Set the point size
    glPointSize(pointSize);

    // Begin drawing points
    glBegin(GL_POINTS);

    // Iterate through the points and draw them
    for (int i = 0;  i < numPoints; i++) {
        rk4_step(&x, &y, &z, 0.1);
        glVertex3f(x, y, z);
    }

    // End drawing points
    glEnd();

    // Pop the current matrix from the matrix stack
    glPopMatrix();

    // Display the information if the flag is set
    if (showInfo) {
        glColor3f(1.0f, 1.0f, 1.0f);
        renderBitmapString(-3.5f, 1.8f, GLUT_BITMAP_HELVETICA_12, infoString);
    }

    // Swap the front and back buffers to display the rendered scene
    glutSwapBuffers();
}

// Function to handle animation
void idle() {
    angleZ += 1; // Increment the rotation angle around the Z axis
    if (angleZ >= 360.0) angleZ -= 360.0; // Wrap the angle within 360 degrees
    glutPostRedisplay(); // Request a redraw of the scene
}

// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ASCII code for the Esc key
        case 'q':
            if (isFullScreen) {
                glutReshapeWindow(960, 540); // Restore the window to its original size
                isFullScreen = 0; // Toggle fullscreen mode off
            } else {
                exit(0);
            }
            break;
        case 'f':
            if (isFullScreen) {
                glutReshapeWindow(960, 540); // Restore the window to its original size
                isFullScreen = 0; // Toggle fullscreen mode off
            } else {
                glutFullScreen(); // Switch to fullscreen mode
                isFullScreen = 1; // Toggle fullscreen mode on
            }
            break;
        case 'r': // Enable random color
            color[0] = (float)rand() / RAND_MAX; // Set a random value for the red component
            color[1] = (float)rand() / RAND_MAX; // Set a random value for the green component
            color[2] = (float)rand() / RAND_MAX; // Set a random value for the blue component
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
            if (numPoints < 100000)
                numPoints += 500;
            break;
        case GLUT_KEY_DOWN: // Decrease the number of points
            if (numPoints > 500) // To prevent the number of points from becoming less than 0
                numPoints -= 500;
            break;
        case GLUT_KEY_RIGHT: // Increase point size
            if (pointSize < 3.0)
                pointSize += 0.1;
            break;
        case GLUT_KEY_LEFT: // Decrease point size
            if (pointSize > 0.2)
                pointSize -= 0.1;
            break;
    }
    sprintf(infoString, "Points: %d, Size: %.1f", numPoints, pointSize);
}

// Function to set up the scene
void setup() {
    // Set the clear color (black)
    glClearColor(0.0, 0.0, 0.0, 1.0);
    // Set the point size for rendering
    glPointSize(pointSize);

    // Set the current matrix mode to projection
    glMatrixMode(GL_PROJECTION);

    // Load the identity matrix to reset the projection matrix
    glLoadIdentity();

    // Set the perspective projection
    gluPerspective(24.0, (double) screenWidth / (double) screenHeight, 1.0, 100.0);

    // Set the current matrix mode to modelview
    glMatrixMode(GL_MODELVIEW);

    // Load the identity matrix to reset the modelview matrix
    glLoadIdentity();

    // Translate the scene along the z-axis to position it properly in the view
    glTranslatef(0.0, 0.0, -10.0);
}

int main(int argc, char** argv) {
    // Initialize GLUT and create a window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Set display mode
    glutInitWindowSize(960, 540); // Set initial window size
    glutCreateWindow("Aizawa Attractor"); // Create a window with the given title
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering

    // Register callback functions for various events
    glutDisplayFunc(display); // Function to render the scene
    sprintf(infoString, "Points: %d, Size: %.1f", numPoints, pointSize);
    glutIdleFunc(idle); // Function to handle idle state
    glutKeyboardFunc(keyboard); // Function to handle keyboard input
    glutSpecialFunc(specialKeys); // Function to handle special keys
    
    glutPostRedisplay(); // Request a redraw of the scene

    setup(); // Initialize the scene

    glutMainLoop(); // Enter the GLUT event processing loop

    return 0;
}
