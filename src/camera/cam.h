#ifndef CAM_H
#define  CAM_H
#include "../objects/hittable.h"
#include "../objects/plane.h"
#include "../objects/hittable_lists.h"
#include "../transform/transform.h"

inline mat4 remove_translation(const mat4& M) {
    mat4 R = M;
    R[0][3] = R[1][3] = R[2][3] = 0.0;
    return R;
}

shared_ptr<hittable> add_object_camera(
    const shared_ptr<hittable>& obj,
    const Transform& T,
    const mat4& Mwc,
    const mat4& Mcw
) {
    Transform Tc = T;
    Tc.M = Mwc * Tc.M;
    Tc.Minv = Tc.Minv * Mcw;
    return make_shared<transform> (
        obj, 
        Tc
    );
}

shared_ptr<plane> add_plane_camera(
    const point4& p,    // ponto do plano
    const vec4& n,      // normal do plano
    const mat4& Mwc,
    shared_ptr<material> mat
) {
    // coverte o ponto do plano em coordenadas de mundo para câmera
    point4 Pc = Mwc * p;

    // converte a normal do plano em coordenadas de mundo para câmera
    mat4 Rwc = remove_translation(Mwc);
    vec4 Nc = unit_vector(Rwc * n);

    return make_shared<plane>(Pc, Nc, mat);
}


#endif