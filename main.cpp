#include <iostream>
#include <algorithm>
#include <GL/freeglut.h>
#include <vector>
#include <memory>
#include <limits> 

//Base Matemática e Raios
#include "src/vectors/vec.h"
#include "src/transform/operations.h"
#include "src/ray/ray.h"

//Cores e Iluminação
#include "src/colors/color.h"
// #include "src/colors/luz_dir.h" // Função substituída pela versão local
#include "src/colors/luz_pontual.h" 
#include "src/colors/luz_spot.h"    

// Objetos e Malhas
#include "src/objects/hittable.h"
#include "src/objects/hittable_lists.h"
#include "src/objects/sphere.h"
#include "src/objects/cilindro.h"
#include "src/objects/cone.h"
#include "src/objects/plane.h"
#include "src/malha/box_mesh.h"

//Transformações e Câmera
#include "src/transform/transform.h"
#include "src/camera/cam.h"

//Cenário
#include "src/cenario/materiais_cenario.h" 
#include "src/cenario/cenario_saojoao.h" 

//Variáveis Globais
int nCol = 500;  
int nLin = 500;
GLubyte* PixelBuffer = nullptr;

//Config da Câmera ---
point4 Eye(250, 150, -200, 1); 
point4 At(250, 80, 400, 1);    
vec4 Up(0, 1, 0, 0);
double dist_focal = 40.0;    

hittable_list world_cam;        

//Função auxiliar para clamp
inline double clamp_val(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

//Função de Traçado de Raios com Fundo Gradiente
color ray_color_with_sky(
    const ray& r,
    const hittable& world,
    const vec4& light_dir, 
    const color& I_A,
    const color& I_F
) {
    hit_record rec;
    const double eps = 1e-4;

    //Renderização do Fundo (Céu)
    // Se o raio não atingir nenhum objeto, calcula a cor baseada na direção Y
    if (!world.hit(r, eps, std::numeric_limits<double>::infinity(), rec)) {
        vec4 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);

        // Interpolação linear para gradiente (Azul Noturno)
        return (1.0 - t) * color(0.1, 0.1, 0.4, 1.0)  // Cor do Horizonte
             + t * color(0.02, 0.02, 0.1, 1.0);     // Cor do Zênite
    }

    //Cálculo do Material e Iluminação (Modelo de Phong)
    color base_color = rec.mat->tex 
        ? rec.mat->tex->value(rec.u, rec.v, rec.p) 
        : rec.mat->K_dif;

    color ambient = I_A * base_color;
    vec4 n = unit_vector(rec.normal);
    vec4 l = unit_vector(light_dir); 

    double nl = dot(n, l);

    // Se a normal estiver oposta à luz, apenas componente ambiente
    if (nl <= 0.0) return ambient; 

    // 3. Cálculo de Sombras
    point4 shadow_origin = rec.p + n * eps;
    ray shadow_ray(shadow_origin, l);
    hit_record shadow_rec;

    // Se o raio de sombra atingir um objeto, o ponto está ocluído
    if (world.hit(shadow_ray, eps, 1e30, shadow_rec)) {
        return ambient;
    }

    //Componentes Difusa e Especular
    vec4 v = unit_vector(-r.direction());
    vec4 rfl = unit_vector(reflect(-l, n));

    double spec = pow(std::max(0.0, dot(v, rfl)), rec.mat->shininess);
    
    color I_d = I_F * base_color * nl;
    color I_e = I_F * rec.mat->K_esp * spec;

    return ambient + I_d + I_e;
}

void raycasting() {
    if(PixelBuffer == nullptr) PixelBuffer = new GLubyte[nCol * nLin * 3];
    // Limpeza do buffer
    for(int i=0; i<nCol*nLin*3; ++i) PixelBuffer[i] = 0;

    double wJanela = 60.0; 
    double hJanela = 60.0;
    double Dx = wJanela / nCol; 
    double Dy = hJanela / nLin;
    
    // Sistema de coordenadas da câmera
    vec4 kc = unit_vector(Eye - At);
    vec4 ic = unit_vector(cross(Up, kc));
    vec4 jc = cross(kc, ic);
    
    // Matriz World-to-Camera
    mat4 Mwc(ic.x(), ic.y(), ic.z(), -dot(ic, Eye),
             jc.x(), jc.y(), jc.z(), -dot(jc, Eye),
             kc.x(), kc.y(), kc.z(), -dot(kc, Eye),
             0,      0,      0,      1); 

    // Matriz Camera-to-World
    mat4 Mcw(ic.x(), jc.x(), kc.x(), Eye.x(),
             ic.y(), jc.y(), kc.y(), Eye.y(),
             ic.z(), jc.z(), kc.z(), Eye.z(),
             0,      0,      0,      1);

    //Config da Luz Direcional (lua)
    vec4 moon_direction_world = unit_vector(vec4(0.2, 1.0, 0.5, 0));
    vec4 moon_direction_cam = unit_vector(Mwc * moon_direction_world);

    // Intensidades da luz (Ambiente e Fonte)
    color I_A(0.3, 0.3, 0.4, 0.0);       
    color I_F(0.7, 0.7, 0.8, 0.0);       

    // Montagem do cenário
    world_cam.clear();
    montar_sao_joao(world_cam, Mwc, Mcw);

    // Loop de renderização
    #pragma omp parallel for
    for (int l = 0; l < nLin; l++) {
        double y = hJanela/2.0 - Dy/2.0 - l*Dy;
        for (int c = 0; c < nCol; c++) {
            double x = -wJanela/2.0 + Dx/2.0 + c*Dx;
            double z = -dist_focal; 

            point4 Pc(x, y, z, 1); 
            point4 Ec(0, 0, 0, 1);  
            vec4 dir = unit_vector(Pc - Ec); 
            ray r(Ec, dir);

            // Chamada da função de cor personalizada
            color pixel_color = ray_color_with_sky(r, world_cam, moon_direction_cam, I_A, I_F);
            
            int flipped_row = nLin - 1 - l;
            int position = (flipped_row * nCol + c) * 3;
            PixelBuffer[position]     = int(255.99 * clamp_val(pixel_color.x(), 0.0, 1.0));
            PixelBuffer[position + 1] = int(255.99 * clamp_val(pixel_color.y(), 0.0, 1.0));
            PixelBuffer[position + 2] = int(255.99 * clamp_val(pixel_color.z(), 0.0, 1.0));
        }
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        std::cout << "Clique: " << x << ", " << y << std::endl;
    }
}

void keyboard(unsigned char key, int mousex, int mousey) {
    if(key == 27) exit(0); 
    
    // Controle de Zoom
    if(key == 'w' || key == 'W') dist_focal += 5; 
    if(key == 's' || key == 'S') dist_focal -= 5; 
    
    if(key == 'w' || key == 's' || key == 'W' || key == 'S') {
        std::cout << "Zoom: " << dist_focal << std::endl;
        raycasting(); 
        glutPostRedisplay();    
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(nCol, nLin, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
    glFlush();
}

void cleanup() { if(PixelBuffer != nullptr) delete[] PixelBuffer; }

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(nCol, nLin);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cenario de Sao Joao - Noite");
    
    std::cout << "Renderizando..." << std::endl;
    raycasting(); 

    glutKeyboardFunc(keyboard); 
    glutMouseFunc(mouse); 
    glutDisplayFunc(display);
    
    atexit(cleanup); 
    glutMainLoop();
    return 0;
}