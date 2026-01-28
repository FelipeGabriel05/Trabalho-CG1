#ifndef CAM_H
#define  CAM_H
#include "../objects/hittable.h"
#include "../objects/plane.h"
#include "../objects/hittable_lists.h"
#include "../transform/transform.h"

// parâmetros da câmera (vindos da main)
extern double xmin, xmax, ymin, ymax;
extern double dJanela;
extern int nCol, nLin;


// pixel (i,j) -> ponto na câmera
inline point4 pixel_to_camera(int i, int j)
{
    double Dx = (xmax - xmin) / nCol;
    double Dy = (ymax - ymin) / nLin;

    double x = xmin + Dx * (j + 0.5);
    double y = ymax - Dy/2.0 - i * Dy;
    double z = -dJanela;

    return point4(x, y, z, 1.0);
}

// Gera o raio a partir do pixel
inline ray generate_ray(int i, int j)
{
    point4 Pc = pixel_to_camera(i, j);
    point4 Oc(0, 0, 0, 1);

    vec4 dir = unit_vector(Pc - Oc);
    return ray(Oc, dir);
}


inline mat4 remove_translation(const mat4& M) {
    mat4 R = M;
    R[0][3] = R[1][3] = R[2][3] = 0.0;
    return R;
}

inline shared_ptr<hittable> add_object_camera(
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

inline shared_ptr<plane> add_plane_camera(
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