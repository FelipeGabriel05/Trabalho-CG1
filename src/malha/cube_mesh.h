#ifndef CUBE_MESH_H
#define CUBE_MESH_H
#include "list_mesh.h"
#include "../material/material.h"
#include "triangulo.h"
#include "../vectors/vec.h"
#include "../objects/hittable.h"
#include <memory>

// Cria um cubo perfeito
class cube_mesh : public hittable{
    public:
        cube_mesh(
            const double aresta_cubo,
            const point4 centroBase,
            std::shared_ptr<material> m
        ) : a(aresta_cubo), center(centroBase), mat(m) {
            build_mesh();
        }

        bool hit(const ray& r, double tmin, double tmax, hit_record& rec)  const override{
            bool hit_any = false;
            double closest = tmax;

            for(const auto& tri : triangles) {
                if(tri->hit(r,tmin,closest,rec)){
                    hit_any = true;
                    closest = rec.t;
                    rec.obj = this;
                }
            }
            return hit_any;
        }

        std::shared_ptr<material> mat;
        std::shared_ptr<material> get_material() const override {
            return mat;
        }

        ObjectType type() const override {
            return ObjectType::Mesh;
        }

    private:
        double a;
        point4 center;
        std::shared_ptr<material> mat;
        std::shared_ptr<ListMesh> mesh;
        std::vector<std::shared_ptr<triangulo>> triangles;
        
        void build_mesh() {
            mesh = std::make_shared<ListMesh>();
            double hx = a * 0.5;
            double hz = a * 0.5;

            // As arestas da base são paralelas aos eixos x e z do sistema de coordenadas
            // para determinar as posições de x e z com base no centro usamos a metade da aresta a e variamos o seu valor para deslocar
            // aos cantos da base. A coordenada Y é fixa

            // Vértices da base (paralelos a X e Z)
            int v0 = mesh->addVertex({center.x() - hx, center.y(), center.z() - hz, 0});
            int v1 = mesh->addVertex({center.x() + hx, center.y(), center.z() - hz, 0});
            int v2 = mesh->addVertex({center.x() + hx, center.y(), center.z() + hz, 0});
            int v3 = mesh->addVertex({center.x() - hx, center.y(), center.z() + hz, 0});

            // Vértice do topo 
            int v4 = mesh->addVertex({center.x() - hx, center.y() + a, center.z() - hz, 0});
            int v5 = mesh->addVertex({center.x() + hx, center.y() + a, center.z() - hz, 0});
            int v6 = mesh->addVertex({center.x() + hx, center.y() + a, center.z() + hz, 0});
            int v7 = mesh->addVertex({center.x() - hx, center.y() + a, center.z() + hz, 0});

            // face da base
            mesh->addFace({v0, v1, v2});
            mesh->addFace({v0, v2, v3});

            // face do topo
            mesh->addFace({v4, v5, v6});
            mesh->addFace({v4, v6, v7});

            // face frontal
            mesh->addFace({v0, v5, v1});
            mesh->addFace({v0, v4, v5});

            // face traseira
            mesh->addFace({v3, v2, v6});
            mesh->addFace({v3, v6, v7});

            // face esquerda
            mesh->addFace({v0, v4, v7});
            mesh->addFace({v0, v7, v3});

            // face direita
            mesh->addFace({v1, v5, v6});
            mesh->addFace({v1, v6, v2});

            // gerar os triângulos
            for(const auto& f : mesh->faces()) {
                triangles.push_back(
                    std::make_shared<triangulo>(
                        f.verts()[0], f.verts()[1], f.verts()[2],
                        mesh, mat
                    )
                );
            }
        };
};
#endif