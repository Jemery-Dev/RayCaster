#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define PI 3.141592653589793238462643383279502884197
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533

float px, py, pdx, pdy, pa; // Player Position

void drawPlayer() {
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(px, py);
    glVertex2i(px + pdx * 5, py + pdy * 5);
    glEnd();
}

void buttons(unsigned char const key, int x, int y) {
    if (key == 'q') {
        pa -= 0.1;
        if (pa < 0) {
            pa += 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'd') {
        pa += 0.1;
        if (pa > 2 * PI) {
            pa -= 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'z') {
        px += pdx;
        py += pdy;
    }
    if (key == 's') {
        px -= pdx;
        py -= pdy;
    }
    glutPostRedisplay();
}

int mapX = 8, mapY = 8, mapS = 64;
int map[] =
{
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,1,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

void drawMap2D() {
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++) {
        for (x = 0; x < mapX; x++) {
            if (map[y * mapX + x] == 1) {
                glColor3f(0.5, 0.7, 0.8);
            }
            else {
                glColor3f(0, 0, 0);
            }
            xo = x * mapS;
            yo = y * mapS;
            glBegin(GL_QUADS);
            glVertex2i(xo + 1, yo + 1);
            glVertex2i(xo + 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + 1);
            glEnd();
        }
    }
}

float dist(float ax, float ay, float bx, float by) {
    return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}

void drawRays3D() {
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo;
    ra = pa-DR*30;
    if(ra<0) {
        ra+=2*PI;
    }
    if(ra>2*PI) {
        ra-=2*PI;
    }
    for (r = 0; r < 1; r++) {
        // Check horizontal lines
        dof = 0;
        float disH = 100000, hx = px, hy = py;
        float aTan = -1 / tan(ra);
        if (ra > PI) { // Looking Up
            ry = (((int)py >> 6) << 6) - 0.0001;
            rx = (py - ry) * aTan + px;
            yo = -64;
            xo = -yo * aTan;
        }
        else if (ra < PI) { // Looking Down
            ry = (((int)py >> 6) << 6) + 64;
            rx = (py - ry) * aTan + px;
            yo = 64;
            xo = -yo * aTan;
        }
        else { // Looking straight left or right
            rx = px;
            ry = py;
            dof = 8;
        }

        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
                hx = rx;
                hy = ry;
                disH = dist(px, py, hx, hy);
                dof = 8; // HIT WALL
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1; // Next line
            }
        }

        // Check vertical lines
        dof = 0;
        float disV = 100000, vx = px, vy = py;
        float nTan = -tan(ra);
        if (ra > P2 && ra < P3) { // Looking left
            rx = (((int)px >> 6) << 6) - 0.0001;
            ry = (px - rx) * nTan + py;
            xo = -64;
            yo = -xo * nTan;
        }
        else if (ra < P2 || ra > P3) { // Looking right
            rx = (((int)px >> 6) << 6) + 64;
            ry = (px - rx) * nTan + py;
            xo = 64;
            yo = -xo * nTan;
        }
        else { // Looking straight up or down
            rx = px;
            ry = py;
            dof = 8;
        }

        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
                vx = rx;
                vy = ry;
                disV = dist(px, py, vx, vy);
                dof = 8; // HIT WALL
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1; // Next line
            }
        }

        if (disV < disH) {
            rx = vx;
            ry = vy;
        } else {
            rx = hx;
            ry = hy;
        }

        glColor3f(1, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();
        ra+=DR;
        if(ra<0) {
            ra+=2*PI;
        }
        if(ra>2*PI) {
            ra-=2*PI;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    drawPlayer();
    drawRays3D();
    glutSwapBuffers();
}

void init() {
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 1024, 512, 0);
    px = 300;
    py = 300;
    pa = 0; // Initialize angle to 0
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 512);
    glutCreateWindow("Jeremy Girard - RayTracer");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(buttons);
    glutMainLoop();
}
