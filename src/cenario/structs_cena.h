#ifndef STRUCTS_CENA_H
#define STRUCTS_CENA_H
#include "../vectors/vec.h"
#include "../material/material.h"

struct tree_data {
    // Esfera
    point4 centro_esfera;
    double raio_esfera;
    std::shared_ptr<material> material_esfera;

    // Cone
    point4 centroBase_cone;
    vec4 dir_cone;
    double altura_cone;
    double raio_cone;
    bool tem_base_cone;
    std::shared_ptr<material> material_cone;

    // Tronco
    point4 centroBase_tronco;
    vec4 dir_tronco;
    double altura_tronco;
    double raio_tronco;
    bool fundo_tronco;
    bool tampa_tronco;
    std::shared_ptr<material> material_tronco;

    // Suporte
    point4 centroBase_suporte;
    vec4 dir_suporte;
    double altura_suporte;
    double raio_suporte;
    bool fundo_suporte;
    bool tampa_suporte;
    std::shared_ptr<material> material_suporte;
};

struct table_data{
    point4 centroBase_tampo;
    double largura_tampo;
    double altura_tampo;
    double profundiade_tampo;
    std::shared_ptr<material> material_tampo;

    point4 centroBase_suporte1;
    double largura_suporte1;
    double altura_suporte1;
    double profundidade_suporte1;
    std::shared_ptr<material> material_suporte1;

    point4 centroBase_suporte2;
    double largura_suporte2;
    double altura_suporte2;
    double profundidade_suporte2;
    std::shared_ptr<material> material_suporte2;
};

struct portico_data{
    point4 centroBase_colunaE;
    double largura_colunaE;
    double altura_colunaE;
    double profundiade_colunaE;
    std::shared_ptr<material> material_colunaE;

    point4 centroBase_colunaD;
    double largura_colunaD;
    double altura_colunaD;
    double profundiade_colunaD;
    std::shared_ptr<material> material_colunaD;

    point4 centroBase_vigaE;
    double largura_vigaE;
    double altura_vigaE;
    double profundiade_vigaE;
    std::shared_ptr<material> material_vigaE;

    point4 centroBase_vigaD;
    double largura_vigaD;
    double altura_vigaD;
    double profundiade_vigaD;
    std::shared_ptr<material> material_vigaD;
};

#endif