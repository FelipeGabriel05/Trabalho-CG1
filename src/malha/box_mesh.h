#ifndef BOX_MESH_H
#define BOX_MESH_H
#include "list_mesh.h"
#include "../material/material.h"
#include "triangulo.h"
#include "../vectors/vec.h"
#include "../objects/hittable.h"
#include <memory>

// Cria um paralelepipedo
class box_mesh : public hittable{
    public:
        box_mesh(
            const point4 centroBase,
            double largura,
            double altura,
            double profundidade,
            std::shared_ptr<material> m
        ) : center(centroBase), l(largura), h(altura), p(profundidade), mat(m) {
            build();
        }

        std::shared_ptr<material> get_material() const override {
            return mat;
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

        ObjectType type() const override {
            return ObjectType::Mesh;
        }


    private:
        point4 center;
        double l;
        double h;
        double p;
        std::shared_ptr<material> mat;
        std::shared_ptr<ListMesh> mesh;
        std::vector<std::shared_ptr<triangulo>> triangles;

        void build() {
            mesh = std::make_shared<ListMesh>();
            
            double half_l = l * 0.5;
            double half_p = p * 0.5;
           
            double cx = center.x();
            double cy = center.y();
            double cz = center.z();

            // Vértices da base (paralelos a X e Z)
            int v0 = mesh->addVertex({cx - half_l, cy, cz - half_p, 0});
            int v1 = mesh->addVertex({cx + half_l, cy, cz - half_p, 0});
            int v2 = mesh->addVertex({cx + half_l, cy, cz + half_p, 0});
            int v3 = mesh->addVertex({cx - half_l, cy, cz + half_p, 0});

            // Vértice do topo 
            int v4 = mesh->addVertex({cx - half_l, cy + h, cz - half_p, 0});
            int v5 = mesh->addVertex({cx + half_l, cy + h, cz - half_p, 0});
            int v6 = mesh->addVertex({cx + half_l, cy + h, cz + half_p, 0});
            int v7 = mesh->addVertex({cx - half_l, cy + h, cz + half_p, 0});

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