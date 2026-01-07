#include <iostream>
#include "src/ray/ray.h"
#include "src/vectors/vec.h"
#include "src/colors/color.h"
#include "src/colors/ray_color.h"
#include "src/objects/hittable.h"
#include "src/objects/hittable_lists.h"
#include "src/objects/plane.h"
#include "src/cenario/materiais_cenario.h"
#include "src/cenario/objetos_cenario.h"
#include "src/cenario/arvore_natal.h"
#include "src/cenario/mesa.h"
#include "src/objects/sphere.h"
#include "src/transform/operations.h"
#include "src/transform/transform.h"

#include <GL/freeglut.h>

//Global
int nCol = 500;                 // número de colunas
int nLin = 500;                 // número de linhas
GLubyte* PixelBuffer = nullptr;

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

void makePixel(int row, int col, const color& pixel_color) {
    int position; 
    int flipped_row; 
    if(0 <= row && row < nLin && 0 <= col && col < nCol) {
        flipped_row = nLin - 1 - row; 
        position = (flipped_row * nCol + col) * 3; 
        PixelBuffer[position] = int(255.999 * clamp(pixel_color.x(), 0.0, 1.0));
        PixelBuffer[position + 1] = int(255.999 * clamp(pixel_color.y(), 0.0, 1.0));
        PixelBuffer[position + 2] = int(255.999 * clamp(pixel_color.z(), 0.0, 1.0));
    } 
}

void raycasting(void){
    // Limpar buffer se já existir
    if(PixelBuffer == nullptr) {
        PixelBuffer = new GLubyte[nCol * nLin * 3];
    }

    // Limpeza obrigatória
    std::fill(
        PixelBuffer,
        PixelBuffer + nCol * nLin * 3,
        0
    );

    // -------- Requisitos da tarefa --------
    double wJanela = 60.0;           // largura da janela (cm)
    double hJanela = 60.0;           // altura da janela (cm)
    double dJanela = 30.0;           // distância da janela do olho(cm)
    
    double Dx = wJanela / nCol;     // Tamanho dos pixels da janela. DX e DY
    double Dy = hJanela / nLin;
    point4 E(0,0,0,1);                // Olho do pintor     

    // glClear(GL_COLOR_BUFFER_BIT);
    hittable_list world;

    // world.add(make_shared<sphere>(point4(0, 144, -200, 1.0), 20, material_esfera));
    auto s = make_shared<sphere>(point4(0, 0, 0, 1.0), 1.0, material_esfera);
    mat4 T = translation(0, 100, -200);
    mat4 Tinv = translation_inverse(0, 100, -200);

    mat4 S = scale(40, 40, 40);
    mat4 Sinv = scale_inv(40,40,40);

    // composição
    mat4 M = T * S;
    mat4 Minv = Sinv * Tinv;
    auto esfera_movida = make_shared<transform>(
        s,   // objeto original
        M,        // matriz direta
        Minv      // matriz inversa
    );
    world.add(esfera_movida);


    //tree_data arvore = criar_arvore();
    //world.add(make_shared<tree>(arvore));
    
    //table_data m = criar_mesa();
    //world.add(make_shared<mesa>(m));
    // Planos do cenário
    world.add(make_shared<plane>(point4(0.0, -150, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 0.0), material_chao));
    world.add(make_shared<plane>(point4(200, -150, 0.0, 1.0), vec4(-1.0, 0.0, 0.0, 0.0), material_plano1));
    world.add(make_shared<plane>(point4(200, -150, -400, 1.0), vec4(0.0, 0.0, 1.0, 0.0), material_plano1));
    world.add(make_shared<plane>(point4(-200, -150, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 0.0), material_plano1));
    world.add(make_shared<plane>(point4(0.0, 150, 0.0, 1.0), vec4(0.0, -1.0, 0.0, 0.0), material_teto));

    // Loop linhas e colunas
    for (int l = 0; l < nLin; l++) {
        double y =  hJanela/2.0 - Dy/2.0 - l*Dy;
        for (int c = 0; c < nCol; c++) {
            // Coordenadas do centro da célula
            double x = -wJanela/2.0 + Dx/2.0 + c*Dx;
            double z = -dJanela;

            point4 pixelPos(x,y,z, 1);
            vec4 dir = unit_vector(pixelPos - E); // direção do raio
            ray r(E, dir);

            /*  Projeção ortográfica
                vec4 dir(0, 0, -1);
                ray r(pixelPos, dir);
            */
            color pixel_color = ray_color(r, world);
            makePixel(l, c, pixel_color);
        }
    }
}

void keyboard(unsigned char key, int mousex, int mousey) {
    if(key == 'w'|| key == 'W')
        exit(0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(nCol, nLin, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
    glFlush();
}

void cleanup() {
    if(PixelBuffer != nullptr) {
        delete[] PixelBuffer;
        PixelBuffer = nullptr;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(nCol, nLin);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Trabalho final");
    raycasting();
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);

    // Registrar função de limpeza
    atexit(cleanup);

    glutMainLoop();
}