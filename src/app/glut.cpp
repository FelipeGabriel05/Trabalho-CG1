#include "glut.h"
#include <cstdlib>
#include "../camera/cam.h"
#include "../objects/cilindro.h"
#include "../objects/sphere.h"
#include "../objects/cone.h"
#include "../objects/plane.h"
#include "../malha/mesh_object.h"

extern int nLin;
extern int nCol;
extern GLubyte* PixelBuffer;
extern hittable_list world_cam;
extern double xmin, xmax, ymin, ymax;
extern double dJanela;
extern bool needs_render;

double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

void makePixel(
    int row,
    int col,
    const color& pixel_color,
    int nLin,
    int nCol,
    GLubyte* PixelBuffer
) {
    if (row < 0 || row >= nLin || col < 0 || col >= nCol) return;

    int flipped_row = nLin - 1 - row;
    int position = (flipped_row * nCol + col) * 3;

    PixelBuffer[position]     = int(255.999 * clamp(pixel_color.x(), 0.0, 1.0));
    PixelBuffer[position + 1] = int(255.999 * clamp(pixel_color.y(), 0.0, 1.0));
    PixelBuffer[position + 2] = int(255.999 * clamp(pixel_color.z(), 0.0, 1.0));
}

void mouse_click(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    std::cout << "\n-----------------------------\n";
    std::cout << "Mouse (tela) -> (x,y) = ("
              << x << ", " << y << ")\n";

    // Conversão correta tela -> pixel lógico
    int l = y;
    int c = x;

    // Mesma lógica do render
    point4 Ec(0, 0, 0, 1);
    point4 Pc = pixel_to_camera(l, c);
    vec4 dir = unit_vector(Pc - Ec);
    ray r(Ec, dir);

    hit_record rec;

    if (!world_cam.hit(
            r,
            0.001,
            std::numeric_limits<double>::infinity(),
            rec))
    {
        std::cout << "Nada selecionado\n";
        return;
    }

    std::cout << "Objeto selecionado:\n";
    std::cout << "t_hit = " << rec.t << "\n";
    const hittable* obj_real =
    dynamic_cast<const transform*>(rec.obj)
        ? dynamic_cast<const transform*>(rec.obj)->object.get()
        : rec.obj;

    auto mat = obj_real->get_material();
    switch (obj_real->type()) {
    case ObjectType::Sphere:
        std::cout << "Esfera\n";
        break;
    case ObjectType::Cilindro:
        std::cout << "Cilindro\n";
        break;
    case ObjectType::Cone:
        std::cout << "Cone\n";
        break;
    case ObjectType::Plane:
        std::cout << "Plano\n";
        break;
    case ObjectType::Mesh:
        std::cout << "Objeto de malha (mesh)\n";
        break;
    default:
        std::cout << "Desconhecido\n";
    }
    needs_render = true;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int) {
    if (key == 'x' || key == 'X')
        exit(0);
}

void cleanup() {
    delete[] PixelBuffer;
    PixelBuffer = nullptr;
}
