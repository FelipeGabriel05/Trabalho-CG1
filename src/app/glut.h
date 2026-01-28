#ifndef GLUT_APP_H
#define GLUT_APP_H

#include <GL/freeglut.h>
#include "../colors/color.h"
#include "../objects/hittable.h"
#include "../objects/hittable_lists.h"
enum class Projecao { PERSPECTIVA, ORTOGRAFICA, OBLIQUA };

enum class CenaCamera {
    CENA1_FRONTAL = 1,
    CENA2_QUINA   = 2,
    CENA3_AEREA   = 3,
    CENA4_NORMAL = 4
};

double clamp(double x, double min, double max);

void makePixel(
    int row,
    int col,
    const color& pixel_color,
    int nLin,
    int nCol,
    GLubyte* PixelBuffer
);

void mouse_click(int button, int state, int x, int y);

// callbacks GLUT (assinaturas)
void keyboard(unsigned char key, int x, int y);
void cleanup();

#endif
