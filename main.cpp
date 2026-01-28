#include <iostream>
#include "src/ray/ray.h"
#include "src/vectors/vec.h"
#include "src/colors/color.h"
#include "src/colors/luz_dir.h"
#include "src/colors/luz_pontual.h"
#include "src/colors/luz_spot.h"

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
#include "src/app/glut.h"

#include "src/camera/cam.h"

#include <GL/freeglut.h>
#define PI 3.14159265358979324

//Global
int nCol = 500;                 // número de colunas
int nLin = 500;                 // número de linhas
hittable_list world_cam;
bool needs_render = true;
GLubyte* PixelBuffer = nullptr;
hittable_list* g_world = nullptr;
double xmin, xmax, ymin, ymax;
double dJanela;

point4 E;
vec4 ic, jc, kc;


void raycasting(void){

    world_cam.clear();

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

    // Definindo os parâmetros (FOV)
    double fov = PI/2;   // 90 graus (campo de visão horizontal)
    double aspect = (double)nCol / (double)nLin;  // razão de aspecto
    dJanela = 30.0;   // distância da janela (plano de projeção)

    double wJanela = 2.0 * dJanela * tan(fov / 2.0);
    double hJanela = wJanela / aspect;

    xmin = -wJanela / 2.0;
    xmax =  wJanela / 2.0;
    ymin = -hJanela / 2.0;
    ymax =  hJanela / 2.0;
    
    // -------- Câmera -------------
    E = point4(0, 0, 0, 1);
    point4 AT(0, 0, -200, 1);
    vec4 Pup(0, 1, 0, 0);

    // Base da câmera
    kc = unit_vector(E - AT);
    ic = unit_vector(cross(Pup, kc));
    jc = cross(kc, ic);

    // -------- CENA --------
    // world_cam.clear();
    
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

    // cena
    g_world = &world_cam;

    auto s = make_shared<sphere>(point4(0, 0, 0, 1.0), 10.0, material_esfera);
    Transform ts;
    // ts.scale(40, 40, 40);
    ts.translate(0, 100, -200);

    world_cam.add(
        add_object_camera(
            s,
            ts,
            Mwc,
            Mcw
        )
    );

    auto co = make_shared<cone>(point4(0, 0, 0, 1.0), vec4(0, 1, 0, 0), 150, 90, true, material_cone);
    Transform Tco;
    Tco.translate(0, -60, -200);
    world_cam.add(
        add_object_camera(
            co, Tco, Mwc, Mcw
        )
    ); 
    
    auto ci = make_shared<cilindro>(point4(0, 0, 0, 1.0), vec4(0, 1, 0, 1), 90, 5, true, false, material_cilindro);
    Transform Tci;
    Tci.translate(0, -150, -200);
    world_cam.add(
        add_object_camera(
            ci, Tci, Mwc, Mcw
        )
    );

    // tree_data arvore = criar_arvore();
    // world_cam.add(make_shared<tree>(arvore));
    
    // world_cam.add(make_shared<box_mesh>(point4(20, -55, -200, 1.0), 100, 20, 30, material_tampo));


    auto box = make_shared<box_mesh>(
    point4(0, 0, 0, 1.0),
    100, 20, 30,
    material_tampo
    );
    Transform tb;
    vec4 eixo = vec4(0, 0, 1, 0);
    tb.rotateArbitrary(eixo, PI / 4);
    // tb.rotateZ(PI / 4);
    tb.translate(20, -55, -200);

    world_cam.add(
        add_object_camera(
            box,
            tb,
            Mwc,
            Mcw
        )
    );

    // auto box1 = make_shared<box_mesh>(
    //     point4(0, 0, 0, 1), 100, 100, 100, material_tampo
    // );
    // Transform tb1;
    // tb1.shear_yz(PI/6);
    // tb1.translate(-10, -30, -150);

    // world_cam.add(
    //     add_object_camera(
    //         box1,
    //         tb1,
    //         Mwc, 
    //         Mcw
    //     )
    // );

    vec4 n = unit_vector(vec4(0, 1, 0, 0)); // plano 45° entre X e Y
        auto box2 = make_shared<box_mesh>(
        point4(0,0,0,1),
        40,40,40,
        material_tampo
    );

    Transform tf1;
    tf1.reflect_arbitrary(n);
    tf1.translate(0, 30, -150);

    Transform tf2 = tf1;
    tf2.reflect_arbitrary(n);
    tf2.translate(40, 0, -150);

    // world_cam.add(add_object_camera(box2, tf1, Mwc, Mcw));
    // world_cam.add(add_object_camera(box2, tf2, Mwc, Mcw));


    // table_data m = criar_mesa();
    // world_cam.add(make_shared<mesa>(m));
    // Planos do cenário

    world_cam.add(add_plane_camera(point4(0.0, -150, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 0.0), Mwc, material_chao));
    world_cam.add(add_plane_camera(point4(200, -150, 0.0, 1.0), vec4(-1.0, 0.0, 0.0, 0.0), Mwc, material_plano1));
    world_cam.add(add_plane_camera(point4(200, -150, -400, 1.0), vec4(0.0, 0.0, 1.0, 0.0), Mwc, material_plano1));
    world_cam.add(add_plane_camera(point4(-200, -150, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 0.0), Mwc, material_plano1));
    world_cam.add(add_plane_camera(point4(0.0, 150, 0.0, 1.0), vec4(0.0, -1.0, 0.0, 0.0), Mwc, material_teto));
    

    color I_A(0.3, 0.3, 0.3, 0.0);
    color I_F(0.7, 0.7, 0.7, 0.0);
    // Loop linhas e colunas
    for (int l = 0; l < nLin; l++) {
        for (int c = 0; c < nCol; c++) {
            point4 Ec(0, 0, 0, 1);
            point4 Pc = pixel_to_camera(l, c);
            vec4 dir = unit_vector(Pc - Ec); // direção do raio
            ray r(Ec, dir);

            /*  Projeção ortográfica
                vec4 dir(0, 0, -1);
                ray r(pixelPos, dir);
            */

            color pixel_color = luz_pontual(r, world_cam, point4(-100, 140, -20, 1), I_A, I_F); // ok
            // color pixel_color = luz_spot(r, world_cam, point4(0, 0, 0, 1), unit_vector(vec4(0, 0, -1, 0)), PI / 6, I_A, I_F);
            // color pixel_color = ray_color_dir(r, world_cam, unit_vector(vec4(0, 0, -1, 0)), I_A, I_F); //ok
            makePixel(l, c, pixel_color, nLin, nCol, PixelBuffer);
        }
    }
}

void display() {
    
    if(needs_render) {
        raycasting();
        needs_render = false;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(nCol, nLin, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(nCol, nLin);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Trabalho final");
    
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMouseFunc(mouse_click);

    // Registrar função de limpeza
    atexit(cleanup);

    glutMainLoop();
}