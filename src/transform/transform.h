#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <memory>
#include "../objects/hittable.h"
#include "../transform/operations.h"

class Transform {
    public:
        mat4 M;
        mat4 Minv;
            
        Transform() : M(), Minv() {}
        
        void translate(double x, double y, double z) {
            M = translation(x, y, z) * M;
            Minv = Minv * translation(-x, -y, -z);
        }

        void scale(double x, double y, double z) {
            M = m_scale(x, y, z) * M;
            Minv = Minv * m_scale_inv(x, y, z);
        }

        void rotateArbitrary(vec4 u, double teta) {
            mat4 R = rotation_arbitrary(u, teta);
            mat4 Rinv = transpose(R);

            M = R * M;
            Minv = Minv * Rinv;
        }

        void rotateX(double a) {
            M = rotationX(a) * M;
            Minv = Minv * rotationX(-a);
        }

        void rotateY(double a) {
            M = rotationY(a) * M;
            Minv = Minv * rotationY(-a);
        }

        void rotateZ(double a) {
            M = rotationZ(a) * M;
            Minv = Minv * rotationZ(-a);
        }

        void shear_xy(double s) {
            M = m_shear_xy(s) * M;
            Minv = Minv * m_shear_xy(-s);
        }

        void shear_xz(double s) {
            M = m_shear_xz(s) * M;
            Minv = Minv * m_shear_xz(-s);
        }

        void shear_yx(double s) {
            M = m_shear_yx(s) * M;
            Minv = Minv * m_shear_yx(-s);
        }

        void shear_yz(double s) {
            M = m_shear_yz(s) * M;
            Minv = Minv * m_shear_yz(-s);
        }

        void shear_zx(double s) {
            M = m_shear_zx(s) * M;
            Minv = Minv * m_shear_zx(-s);
        };

        void shear_zy(double s) {
            M = m_shear_zy(s) * M;
            Minv = Minv * m_shear_zy(-s);
        }

        void reflect_arbitrary(vec4 n) {
            // mat4 R = m_reflect_arbitrary(n);
            // M = R * M;
            // Minv = Minv * R; // reflexão é sua própria inversa
            M = m_reflect_arbitrary(n) * M;
            Minv = M;
        }
};

class transform : public hittable {
public:
    std::shared_ptr<hittable> object;
    mat4 M;      // matriz direta
    mat4 Minv;   // matriz inversa

    transform(std::shared_ptr<hittable> obj, const Transform& T)
        : object(obj), M(T.M), Minv(T.Minv) {}

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

    std::shared_ptr<material> get_material() const override {
        return object->get_material();
    }

    ObjectType type() const override {
        return object->type();
    }
};


#endif