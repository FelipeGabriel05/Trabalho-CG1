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

inline mat4 translation_inverse(double tx, double ty, double tz) {
    mat4 T;
    T[0][3] = -tx;
    T[1][3] = -ty;
    T[2][3] = -tz;
    return T;
}

inline mat4 scale(double sx, double sy, double sz) {
    mat4 S;
    S[0][0] = sx;
    S[1][1] = sy;
    S[2][2] = sz;
    return S;
}

inline mat4 scale_inv(double sx, double sy, double sz) {
    mat4 S;
    S[0][0] = 1.0 / sx;
    S[1][1] = 1.0 / sy;
    S[2][2] = 1.0 / sz;
    return S;
}

#endif