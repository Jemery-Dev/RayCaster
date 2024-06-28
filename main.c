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

typedef struct {
    int z,q,s,d;
}ButtonsKeys;

ButtonsKeys Keys;

int mapX = 8, mapY = 8, mapS = 64;
int map[] =
{
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,1,1,1,1,0,1,
    1,0,1,0,0,0,0,1,
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
    float rx, ry, ra, xo, yo, disT;
    ra = pa-DR*30;
    if(ra<0) {
        ra+=2*PI;
    }
    if(ra>2*PI) {
        ra-=2*PI;
    }
    for (r = 0; r < 60; r++) {
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
            if (mp > 0 && mp < mapX * mapY && map[mp] > 0) {
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
            if (mp > 0 && mp < mapX * mapY && map[mp] > 0) {
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
            disT = disV;
            glColor3f(0.9, 0, 0);
        } else {
            rx = hx;
            ry = hy;
            disT = disH;
            glColor3f(0.7, 0, 0);
        }

        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();

        // Draw 3D Walls
        float ca=pa-ra;
        if(ca<0) {
            ca+=2*PI;
        }
        if(ca>2*PI) {
            ca-=2*PI;
        }

        disT=disT*cos(ca); // Fix FishEye effect

        float lineH = (mapS*320)/disT; //Get wall size by distance
        if(lineH>320){
            lineH=320;
        }
        float lineO=160-lineH/2; // Add Offset
        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(r*8+530,lineO);
        glVertex2i(r*8+530, lineH+lineO);
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

float frame1, frame2, fps;

void display() {
    frame2 = glutGet(GLUT_ELAPSED_TIME);
    fps = (frame2 - frame1);
    frame1 = glutGet(GLUT_ELAPSED_TIME);

    if (Keys.q == 1) {
        pa -= 0.1;
        if (pa < 0) {
            pa += 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (Keys.d == 1) {
        pa += 0.1;
        if (pa > 2 * PI) {
            pa -= 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }

    int xo = (pdx < 0) ? -20 : 20;
    int yo = (pdy < 0) ? -20 : 20;

    int ipx = px / 64.0;
    int ipx_add_xo = (px + xo) / 64.0;
    int ipx_sub_xo = (px - xo) / 64.0;
    int ipy = py / 64.0;
    int ipy_add_yo = (py + yo) / 64.0;
    int ipy_sub_yo = (py - yo) / 64.0;

    float speed = 0.2;

    if (Keys.z == 1) {
        if (map[ipy * mapX + ipx_add_xo] == 0) {
            px += pdx * 0.2 * fps * speed;
        }
        if (map[ipy_add_yo * mapX + ipx] == 0) {
            py += pdy * 0.2 * fps * speed;
        }
    }
    if (Keys.s == 1) {
        if (map[ipy * mapX + ipx_sub_xo] == 0) {
            px -= pdx * 0.2 * fps * speed;
        }
        if (map[ipy_sub_yo * mapX + ipx] == 0) {
            py -= pdy * 0.2 * fps * speed;
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    drawPlayer();
    drawRays3D();
    glutSwapBuffers();
}


void ButtonDown(unsigned char key, int x, int y) {
    if(key=='q') {
        Keys.q=1;
    }
    if(key=='d') {
        Keys.d=1;
    }
    if(key=='z') {
        Keys.z=1;
    }
    if(key=='s') {
        Keys.s=1;
    }
    glutPostRedisplay();
}

void ButtonUp(unsigned char key, int x, int y) {
    if(key=='q') {
        Keys.q=0;
    }
    if(key=='d') {
        Keys.d=0;
    }
    if(key=='z') {
        Keys.z=0;
    }
    if(key=='s') {
        Keys.s=0;
    }
    glutPostRedisplay();
}

void resize() {

    glutReshapeWindow(1024,512);
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

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Jeremy Girard - RayTracer");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(ButtonDown);
    glutKeyboardUpFunc(ButtonUp);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
}


