#include <GL/freeglut.h>  // Inclure les en-têtes de FreeGLUT
#include <GL/gl.h>        // Inclure les en-têtes d'OpenGL

// Fonction pour dessiner à l'écran
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Efface le tampon de couleur

    glBegin(GL_TRIANGLES);  // Début du dessin de triangles
    glColor3f(1.0, 0.0, 0.0);  // Couleur rouge
    glVertex2f(-0.5, -0.5);   // Premier sommet
    glColor3f(0.0, 1.0, 0.0);  // Couleur verte
    glVertex2f(0.5, -0.5);    // Deuxième sommet
    glColor3f(0.0, 0.0, 1.0);  // Couleur bleue
    glVertex2f(0.0, 0.5);     // Troisième sommet
    glEnd();  // Fin du dessin de triangles

    glFlush();  // Forcer l'affichage du tampon de dessin
}

// Fonction principale
int main(int argc, char** argv) {
    glutInit(&argc, argv);                      // Initialiser FreeGLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Mode d'affichage
    glutInitWindowSize(400, 400);                // Taille de la fenêtre
    glutInitWindowPosition(100, 100);            // Position de la fenêtre
    glutCreateWindow("OpenGL Test");             // Créer la fenêtre avec un titre

    glClearColor(1.0, 1.0, 1.0, 1.0);  // Couleur de fond blanche
    glutDisplayFunc(display);         // Définir la fonction de rendu

    glutMainLoop();  // Entrer dans la boucle principale de GLUT

    return 0;
}