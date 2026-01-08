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

#include "src/camera/cam.h"

#include <GL/freeglut.h>
#define PI 3.14159265358979324

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
    
    // -------- Câmera -------------
    point4 E(0, 0, 0, 1);
    point4 AT(0, 0, -200, 1);
    vec4 Pup(0, 1, 0, 0);

    // Base da câmera
    vec4 kc = unit_vector(E - AT);
    vec4 ic = unit_vector(cross(Pup, kc));
    vec4 jc = cross(kc, ic);
    
    // Matriz mundo -> câmera
    mat4 Mwc (
        ic.x(), ic.y(), ic.z(), -dot(ic, E),
        jc.x(), jc.y(), jc.z(), -dot(jc, E),
        kc.x(), kc.y(), kc.z(), -dot(kc, E),
        0,      0,      0,      1
    ); 

    mat4 Mcw(
        ic.x(), jc.x(), kc.x(), E.x(),
        ic.y(), jc.y(), kc.y(), E.y(),
        ic.z(), jc.z(), kc.z(), E.z(),
        0,      0,      0,      1
    );

    // Definição de luz
    point4 light_pos(-100, 140, -20, 1);
    point4 light_c = Mwc * light_pos;
    color I_A(0.3, 0.3, 0.3, 0.0);
    color I_F(0.7, 0.7, 0.7, 0.0);

    // cena
    hittable_list world_cam;

    auto s = make_shared<sphere>(point4(0, 0, 0, 1.0), 1.0, material_esfera);
    mat4 T = translation(0, 100, -200);
    mat4 Tinv = translation_inverse(0, 100, -200);

    mat4 S = scale(40, 40, 40);
    mat4 Sinv = scale_inv(40,40,40);

    // composição
    mat4 M = T * S;
    mat4 Minv = Sinv * Tinv;
    world_cam.add(
        add_object_camera(
            s,
            M,
            Minv,
            Mwc,
            Mcw
        )
    );

    //tree_data arvore = criar_arvore();
    //world_cam.add(make_shared<tree>(arvore));
    
    // world_cam.add(make_shared<box_mesh>(point4(20, -55, -200, 1.0), 100, 20, 30, material_tampo));


    auto box = make_shared<box_mesh>(
    point4(0, 0, 0, 1.0),
    100, 20, 30,
    material_tampo
    );
    mat4 T1 = translation(20, -55, -200);
    mat4 Tinv1 = translation_inverse(20, -55, -200);

    double ang = (PI / 4);
    mat4 R = rotationZ(ang);
    mat4 Rinv = rotationZ_inv(ang);

    mat4 M1    = T1 * R;
    mat4 Minv1 = Rinv * Tinv1;

    mat4 M_cam1    = Mwc * M1;
    mat4 Minv_cam1 = Minv1 * Mcw;
    auto box_rot = make_shared<transform>(
        box,        // objeto original
        M_cam1,     // matriz direta
        Minv_cam1   // matriz inversa
    );
    world_cam.add(
        add_object_camera(
            box,
            M1,
            Minv1,
            Mwc,
            Mcw
        )
    );
    world_cam.add(box_rot);

    // table_data m = criar_mesa();
    // world_cam.add(make_shared<mesa>(m));
    // Planos do cenário

    world_cam.add(add_plane_camera(point4(0.0, -150, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 0.0), Mwc, material_chao));
    world_cam.add(add_plane_camera(point4(200, -150, 0.0, 1.0), vec4(-1.0, 0.0, 0.0, 0.0), Mwc, material_plano1));
    world_cam.add(add_plane_camera(point4(200, -150, -400, 1.0), vec4(0.0, 0.0, 1.0, 0.0), Mwc, material_plano1));
    world_cam.add(add_plane_camera(point4(-200, -150, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 0.0), Mwc, material_plano1));
    world_cam.add(add_plane_camera(point4(0.0, 150, 0.0, 1.0), vec4(0.0, -1.0, 0.0, 0.0), Mwc, material_teto));


    // Loop linhas e colunas
    for (int l = 0; l < nLin; l++) {
        double y =  hJanela/2.0 - Dy/2.0 - l*Dy;
        for (int c = 0; c < nCol; c++) {
            // Coordenadas do centro da célula
            double x = -wJanela/2.0 + Dx/2.0 + c*Dx;
            double z = -dJanela;

            point4 Pc(x,y,z, 1);
            point4 Ec(0, 0, 0, 1);
            vec4 dir = unit_vector(Pc - Ec); // direção do raio
            ray r(Ec, dir);

            /*  Projeção ortográfica
                vec4 dir(0, 0, -1);
                ray r(pixelPos, dir);
            */
            color pixel_color = ray_color(r, world_cam, light_c, I_A, I_F);
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