#ifndef CENARIO_SAOJOAO_H
#define CENARIO_SAOJOAO_H

#include "../vectors/vec.h"
#include "../objects/hittable_lists.h"
#include "../objects/sphere.h"
#include "../objects/cilindro.h"
#include "../objects/cone.h"
#include "../objects/plane.h"
#include "../malha/box_mesh.h"
#include "../transform/transform.h"
#include "../camera/cam.h"
#include "materiais_cenario.h"
#include <cmath>
#include <vector>

#ifndef PI
#define PI 3.14159265358979324
#endif

void montar_sao_joao(hittable_list& world, const mat4& Mwc, const mat4& Mcw) {

    //materiais
    auto mat_parede = std::make_shared<material>(color(0.9, 0.9, 0.9, 0), color(0.9, 0.9, 0.9, 0), color(0.1, 0.1, 0.1, 0), 5);
    auto mat_telhado = std::make_shared<material>(color(0.8, 0.4, 0.1, 0), color(0.8, 0.4, 0.1, 0), color(0, 0, 0, 0), 0);
    auto mat_madeira = std::make_shared<material>(color(0.5, 0.3, 0.1, 0), color(0.5, 0.3, 0.1, 0), color(0, 0, 0, 0), 0);
    auto mat_porta = std::make_shared<material>(color(0.4, 0.2, 0.1, 0), color(0.4, 0.2, 0.1, 0), color(0, 0, 0, 0), 0);
    
    //cinza para os postes
    auto mat_poste = std::make_shared<material>(color(0.2, 0.2, 0.2, 0), color(0.2, 0.2, 0.2, 0), color(0, 0, 0, 0), 0);

    //Material da Lâmpada (Emissão/Especular)
    auto mat_lampada = std::make_shared<material>(
        color(1.0, 0.9, 0.4, 0), // Cor difusa (Amarelo Quente)
        color(1.0, 1.0, 1.0, 0), // Cor especular (Branco)
        color(0.1, 0.1, 0.1, 0), 
        100                      // Shininess alto para brilho intenso
    );


    //chão
    world.add(add_plane_camera(point4(250, 0, 250, 1), vec4(0, 1, 0, 0), Mwc, material_chao));

    //config central
    double cx = 250; 
    double cz = 400; 

    //igreja
    double larg_1 = 160; double alt_1 = 80; double prof_1 = 90;
    double larg_2 = 100; double alt_2 = 90; double prof_2 = 80;

    // Andar 1
    auto andar1 = std::make_shared<box_mesh>(point4(0,0,0,1), larg_1, alt_1, prof_1, mat_parede);
    Transform t_a1; t_a1.translate(cx, (alt_1 / 2.0) - 7.0, cz); 
    world.add(add_object_camera(andar1, t_a1, Mwc, Mcw));

    // Andar 2
    auto andar2 = std::make_shared<box_mesh>(point4(0,0,0,1), larg_2, alt_2, prof_2, mat_parede);
    Transform t_a2; t_a2.translate(cx, 108.0, cz); 
    world.add(add_object_camera(andar2, t_a2, Mwc, Mcw));

    // Telhado
    double alt_telhado = 60;
    double raio_telhado = (larg_2 / 2.0) + 15; 
    auto telhado_geo = std::make_shared<cone>(point4(0, 0, 0, 1), vec4(0, 1, 0, 0), alt_telhado, raio_telhado, true, mat_telhado);
    Transform t_telhado; t_telhado.translate(cx, 196.0, cz); 
    world.add(add_object_camera(telhado_geo, t_telhado, Mwc, Mcw));

    // Cruz
    double y_base_cruz = 237.0; 
    auto cruz_v = std::make_shared<box_mesh>(point4(0,0,0,1), 5, 40, 5, mat_madeira);
    Transform tv; tv.translate(cx, y_base_cruz + 20, cz); 
    world.add(add_object_camera(cruz_v, tv, Mwc, Mcw));

    auto cruz_h = std::make_shared<box_mesh>(point4(0,0,0,1), 30, 5, 5, mat_madeira);
    Transform th; th.translate(cx, y_base_cruz + 35, cz); 
    world.add(add_object_camera(cruz_h, th, Mwc, Mcw));

    //fogueira apagada
    double fogueira_z = 180;
    for (int a = 0; a < 360; a += 45) {
        auto lenha = std::make_shared<cilindro>(point4(0,0,0,1), vec4(0,1,0,0), 35, 4, true, true, mat_madeira);
        Transform t_lenha;
        t_lenha.rotateZ(PI / 2.8);
        t_lenha.rotateY(a * PI / 180.0);
        t_lenha.translate(cx, 1, fogueira_z); 
        world.add(add_object_camera(lenha, t_lenha, Mwc, Mcw));
    }

    //Porta
    double larg_porta = 40; double alt_porta = 55; double prof_porta = 10; 
    auto porta = std::make_shared<box_mesh>(point4(0,0,0,1), larg_porta, alt_porta, prof_porta, mat_porta);
    Transform t_porta; t_porta.translate(cx, (alt_porta / 2.0) - 7.0, cz - (prof_1 / 2.0));
    world.add(add_object_camera(porta, t_porta, Mwc, Mcw));


    //postes com as lâmpadas
    
    double alt_poste = 160.0; 
    double raio_poste = 4.0;  
    double raio_esfera = 10.0; 

    //Cálculo da altura do cilindro (centro geométrico)
    double y_poste_centro = (alt_poste / 2.0) - 7.0; 
    
    //cálculo da altura da esfera (topo do poste + raio da esfera)
    double y_topo_esfera = (alt_poste - 7.0) + raio_esfera; 

    auto poste_geo = std::make_shared<cilindro>(point4(0,0,0,1), vec4(0,1,0,0), alt_poste, raio_poste, true, true, mat_poste);
    auto lampada_geo = std::make_shared<sphere>(point4(0,0,0,1), raio_esfera, mat_lampada);

    //Posições
    double z1 = 450.0; double x1 = 130.0; // Fundo
    double z2 = 200.0; double x2 = 240.0; // Frente

    // --- CRIANDO OS OBJETOS ---

    //postes par Fundo (Esq/Dir)
    Transform t_p1e; t_p1e.translate(cx - x1, y_poste_centro, z1);
    world.add(add_object_camera(poste_geo, t_p1e, Mwc, Mcw));
    
    Transform t_l1e; t_l1e.translate(cx - x1, y_topo_esfera, z1); 
    world.add(add_object_camera(lampada_geo, t_l1e, Mwc, Mcw));

    Transform t_p1d; t_p1d.translate(cx + x1, y_poste_centro, z1);
    world.add(add_object_camera(poste_geo, t_p1d, Mwc, Mcw));
    
    Transform t_l1d; t_l1d.translate(cx + x1, y_topo_esfera, z1); 
    world.add(add_object_camera(lampada_geo, t_l1d, Mwc, Mcw));

    // postes par Frente (Esq/Dir)
    Transform t_p2e; t_p2e.translate(cx - x2, y_poste_centro, z2);
    world.add(add_object_camera(poste_geo, t_p2e, Mwc, Mcw));
    
    Transform t_l2e; t_l2e.translate(cx - x2, y_topo_esfera, z2); 
    world.add(add_object_camera(lampada_geo, t_l2e, Mwc, Mcw));

    Transform t_p2d; t_p2d.translate(cx + x2, y_poste_centro, z2);
    world.add(add_object_camera(poste_geo, t_p2d, Mwc, Mcw));
    
    Transform t_l2d; t_l2d.translate(cx + x2, y_topo_esfera, z2); 
    world.add(add_object_camera(lampada_geo, t_l2d, Mwc, Mcw));


    //banderinhas 
    auto mat_b_vermelho = std::make_shared<material>(color(0.9, 0.1, 0.1, 0), color(0.9, 0.1, 0.1, 0), color(0,0,0,0), 0);
    auto mat_b_verde    = std::make_shared<material>(color(0.1, 0.8, 0.1, 0), color(0.1, 0.8, 0.1, 0), color(0,0,0,0), 0);
    auto mat_b_amarelo  = std::make_shared<material>(color(0.9, 0.9, 0.1, 0), color(0.9, 0.9, 0.1, 0), color(0,0,0,0), 0);
    auto mat_b_azul     = std::make_shared<material>(color(0.1, 0.2, 0.9, 0), color(0.1, 0.2, 0.9, 0), color(0,0,0,0), 0);

    // Corda conectada na altura da lâmpada
    double y_corda = y_topo_esfera; 

    //Laterais
    int total_lat = 20; 
    for (int lado = 0; lado < 2; lado++) { 
        for (int i = 0; i <= total_lat; i++) {
            double t = (double)i / total_lat;
            double sinal = (lado == 0) ? -1.0 : 1.0;
            
            double bx = (1.0 - t) * (cx + (sinal * x1)) + t * (cx + (sinal * x2));
            double bz = (1.0 - t) * z1 + t * z2;
            double by = y_corda - (12.0 * sin(t * PI)); 

            auto mat_atual = mat_b_vermelho;
            if (i % 4 == 1) mat_atual = mat_b_verde;
            if (i % 4 == 2) mat_atual = mat_b_amarelo;
            if (i % 4 == 3) mat_atual = mat_b_azul;

            auto bandeira = std::make_shared<box_mesh>(point4(0,0,0,1), 5, 5, 1, mat_atual);
            Transform t_ban; t_ban.rotateZ(PI / 4.0); t_ban.translate(bx, by, bz);
            if (i > 0 && i < total_lat) world.add(add_object_camera(bandeira, t_ban, Mwc, Mcw));
        }
    }

    //fundo
    int total_fundo = 25; 
    for (int i = 0; i <= total_fundo; i++) {
        double t = (double)i / total_fundo;
        double bx = (1.0 - t) * (cx - x1) + t * (cx + x1);
        double bz = z1; 
        double by = y_corda - (16.0 * sin(t * PI)); 

        auto mat_atual = mat_b_vermelho;
        if (i % 4 == 1) mat_atual = mat_b_verde;
        if (i % 4 == 2) mat_atual = mat_b_amarelo;
        if (i % 4 == 3) mat_atual = mat_b_azul;

        auto bandeira = std::make_shared<box_mesh>(point4(0,0,0,1), 5, 5, 1, mat_atual);
        Transform t_ban; t_ban.rotateZ(PI / 4.0); t_ban.translate(bx, by, bz);
        if (i > 0 && i < total_fundo) world.add(add_object_camera(bandeira, t_ban, Mwc, Mcw));
    }

   
}

#endif