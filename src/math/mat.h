#pragma once

#include "vec.h"

template <int m, int n>
struct Mat{
    float arr[m][n];
};

using Mat3 = Mat<3,3>;
using Mat4 = Mat<4,4>;


template <int p, int q, int r>
Mat<p,r> operator*(Mat<p,q> a, Mat <q,r> b){
    Mat<p,r> m;
    for(int i=0;i<p;i++){
        for(int j=0; j<r;j++){
            m.arr[i][j] = 0;
            for(int k=0; k<q;k++){
                m.arr[i][j] += a.arr[i][k] * b.arr[k][j];
            }
        }
    }
    return m;
}

template <int p, int q>
Mat<q,p> transpose(Mat<p,q> a){
    Mat<q,p> b;
    for(int i=0;i<p;i++){
        for(int j=0; j<q;j++){
            b.arr[j][i] = a.arr[i][j];
        }
    }
    return(b);
}

template <int p>
Mat<p,p> identity(){
    Mat<p,p> m = {};
    for(int i=0; i<p; i++){
        m.arr[i][i] = 1;
    }
    return m;
}

Vec3f operator *(Mat3 m, Vec3f a);
Vec4f operator *(Mat4 m, Vec4f a);


Mat3 scaleAboutOrigin2D(float sx, float sy);
Mat3 translate2D(float tx, float ty);
Mat3 rotateAboutOrigin2D(float degrees);
Mat3 rotateAboutPoint2D(float degrees, Vec2f pivot);


Mat4 scaleAboutOrigin(float sx, float sy, float sz);
Mat4 translate(float tx, float ty, float tz);
