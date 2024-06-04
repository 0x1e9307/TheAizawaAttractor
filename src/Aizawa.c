#include <GL/glut.h>
#include <math.h>
#include <stdlib.h> // Для функции exit()

// Параметры аттрактора Айзавы
double a = 0.95, b = 0.7, c = 0.6, d = 3.5, e = 0.25, f = 0.1;
double x = 0.1, y = 0, z = 0;

// Угол поворота вокруг оси Z
double angleZ = 0.0;

// Количество точек аттрактора
int numPoints = 40000;

// Переключатель полноэкранного режима
int isFullScreen = 0;

// Цвет аттрактора
float color[3] = {0.0, 1.0, 1.0};

// Размеры экрана
int screenWidth = 1920;
int screenHeight = 1080;

// Функция для численного интегрирования методом Рунге-Кутты 4-го порядка
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0); // Поворот на 90 градусов по оси X
    glRotatef(angleZ, 0.0, 0.0, 1.0); // Вращение вокруг оси Z
    glColor3fv(color); // Устанавливаем текущий цвет
    glBegin(GL_POINTS);
    for (int i = 0; i < numPoints; i++) {
        rk4_step(&x, &y, &z, 0.1);
        glVertex3f(x, y, z);
    }
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}

void setup() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(24.0, (double) screenWidth / (double) screenHeight, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
}

void idle() {
    angleZ += 1;
    if (angleZ >= 360.0) angleZ -= 360.0;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ASCII код клавиши Esc
        case 'q':
            if (isFullScreen) {
                glutReshapeWindow(960, 540);
                isFullScreen = 0;
            } else {
                exit(0);
            }
            break;
        case 'f':
            if (!isFullScreen) {
                glutFullScreen();
                isFullScreen = 1;
            }
            break;
        case 'r': // Включение случайного цвета
            color[0] = (float)rand() / RAND_MAX;
            color[1] = (float)rand() / RAND_MAX;
            color[2] = (float)rand() / RAND_MAX;
            break;
        case 'R': // Сброс цвета в изначальное состояние
            color[0] = 0.0;
            color[1] = 1.0;
            color[2] = 1.0;
            break;
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: // Увеличение количества точек
            numPoints += 1000;
            break;
        case GLUT_KEY_DOWN: // Уменьшение количества точек
            if (numPoints > 1000) // Чтобы точек не стало меньше 0
                numPoints -= 1000;
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(960, 540);
    glutCreateWindow("Aizawa Attractor");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys); // Регистрация функции обработки специальных клавиш
    setup();
    glutMainLoop();
    return 0;
}

