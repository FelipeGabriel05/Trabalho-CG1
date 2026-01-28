#ifndef GLUT_APP_H
#define GLUT_APP_H

#include <GL/freeglut.h>
#include "../colors/color.h"
#include "../objects/hittable.h"
#include "../objects/hittable_lists.h"

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

// callbacks GLUT (assinaturas corretas!)
void keyboard(unsigned char key, int x, int y);
void cleanup();

#endif
