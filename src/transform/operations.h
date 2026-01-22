#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "../objects/hittable.h"
#include "../vectors/vec.h"

class mat4 {
    public:
        double m[4][4];

        mat4() {
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 4; j++) {
                    if(i == j) {
                        m[i][j] = 1;
                    } else {
                        m[i][j] = 0;
                    }
                }
            }
        }

        mat4 (
            double a, double b, double c, double d,
            double e, double f, double g, double h,
            double i, double j, double k, double l,
            double m_, double n, double o, double p
        ) {
           m[0][0] = a; m[0][1] = b; m[0][2] = c; m[0][3] = d;
           m[1][0] = e; m[1][1] = f; m[1][2] = g; m[1][3] = h;
           m[2][0] = i; m[2][1] = j; m[2][2] = k; m[2][3] = l;
           m[3][0] = m_; m[3][1] = n; m[3][2] = o; m[3][3] = p;
        }

        double* operator[](int i) { return m[i]; }
        const double* operator[](int i) const { return m[i]; }

};

inline mat4 operator*(const mat4& A, const mat4& B) {
    mat4 C;
    double res;
        for(int k = 0; k < 4; k++) {
            for(int i = 0; i < 4; i++) {
                res = 0;
                for(int j = 0; j < 4; j++) {
                    res += A[i][j] * B[j][k];
                }
            C[i][k] = res;
            }
        }
    return C;
}

inline vec4 operator*(const mat4& M, const vec4& v) {
    return vec4(
        M[0][0]*v[0] + M[0][1]*v[1] + M[0][2]*v[2] + M[0][3]*v[3],
        M[1][0]*v[0] + M[1][1]*v[1] + M[1][2]*v[2] + M[1][3]*v[3],
        M[2][0]*v[0] + M[2][1]*v[1] + M[2][2]*v[2] + M[2][3]*v[3],
        M[3][0]*v[0] + M[3][1]*v[1] + M[3][2]*v[2] + M[3][3]*v[3]
    );
}

inline mat4 transpose(const mat4& M) {
    mat4 T;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            T[i][j] = M[j][i];
        }
    }
    return T;
}
 
inline mat4 translation(double tx, double ty, double tz) {
    mat4 T;
    T[0][3] = tx;
    T[1][3] = ty;
    T[2][3] = tz;
    return T;
}

inline mat4 m_scale(double sx, double sy, double sz) {
    mat4 S;
    S[0][0] = sx;
    S[1][1] = sy;
    S[2][2] = sz;
    return S;
}

inline mat4 rotationX(double angulo) {
    mat4 R;
    double c = cos(angulo);
    double s = sin(angulo);

    R[1][1] = c;
    R[1][2] = -s;
    R[2][1] = s;
    R[2][2] =  c;
    return R;
}

inline mat4 rotationY(double angulo) {
    mat4 R;
    double c = cos(angulo);
    double s = sin(angulo);

    R[0][0] = c;
    R[0][2] = s;
    R[2][0] = -s;
    R[2][2] = c;
    return R;
}

inline mat4 rotationZ(double angulo) {
    mat4 R;
    double c = cos(angulo);
    double s = sin(angulo);

    R[0][0] = c;
    R[0][1] = -s;
    R[1][0] = s;
    R[1][1] = c;
    return R;
}

inline mat4 m_scale_inv(double sx, double sy, double sz) {
    mat4 S;
    S[0][0] = 1.0 / sx;
    S[1][1] = 1.0 / sy;
    S[2][2] = 1.0 / sz;
    return S;
}

inline mat4 m_shear_xy(double xy) {
    mat4 Sh;
    Sh[0][1] = tan(xy);
    return Sh;
}

inline mat4 m_shear_xz(double xz) {
    mat4 Sh;
    Sh[0][2] = tan(xz);
    return Sh;
}

inline mat4 m_shear_yx(double yx) {
    mat4 Sh;
    Sh[1][0] = tan(yx);
    return Sh;
}

inline mat4 m_shear_yz(double yz) {
    mat4 Sh;
    Sh[1][2] = tan(yz);
    return Sh;
}

inline mat4 m_shear_zx(double zx) {
    mat4 Sh;
    Sh[2][0] = tan(zx);
    return Sh;
}

inline mat4 m_shear_zy(double zy) {
    mat4 Sh;
    Sh[2][1] = tan(zy);
    return Sh;
}

inline mat4 m_reflect_arbitrary(vec4 n) {
    mat4 R;

    double nx = n.x();
    double ny = n.y();
    double nz = n.z();

    R[0][0] = 1 - 2*nx*nx; 
    R[0][1] = -2 * nx * ny;
    R[0][2] = -2 * nx * nz;

    R[1][0] = -2 * ny * nx;
    R[1][1] = 1 - 2 * ny*ny;
    R[1][2] = -2 * ny * nz;

    R[2][0] = -2 * nz * nx; 
    R[2][1] = -2 * nz * ny;
    R[2][2] = 1 - 2 * nz * nz;

    return R;
}

#endif