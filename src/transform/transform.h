#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <memory>
#include "../objects/hittable.h"
#include "../transform/operations.h"


class transform : public hittable {
public:
    std::shared_ptr<hittable> object;
    mat4 M;      // matriz direta
    mat4 Minv;   // matriz inversa

    transform(std::shared_ptr<hittable> obj,
              const mat4& m,
              const mat4& minv)
        : object(obj), M(m), Minv(minv) {}

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        // transforma o raio para o espaço do objeto
       vec4 o = Minv * vec4(
            r.origin().x(),
            r.origin().y(),
            r.origin().z(),
            1.0              // ponto
        );

        vec4 d = Minv * vec4(
            r.direction().x(),
            r.direction().y(),
            r.direction().z(),
            0.0              // vetor
        );

        ray r_obj(o, d);

        if (!object->hit(r_obj, t_min, t_max, rec)) {
            return false;
        }

        rec.p = M * rec.p;

        mat4 normal_mat = transpose(Minv);
        vec4 n = normal_mat *  vec4(
            rec.normal.x(),
            rec.normal.y(),
            rec.normal.z(),
            0.0
        );
        rec.normal = unit_vector(n);
        return true;
    }
};


#endif