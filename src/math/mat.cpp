#include "./mat.h"
#include "./utils.h"
#include <math.h>

Vec3f operator *(Mat3 m, Vec3f a){
    Vec3f b;
    b.x = m.arr[0][0] * a.x + m.arr[0][1] * a.y + m.arr[0][2] * a.z;
    b.y = m.arr[1][0] * a.x + m.arr[1][1] * a.y + m.arr[1][2] * a.z;
    b.z = m.arr[2][0] * a.x + m.arr[2][1] * a.y + m.arr[2][2] * a.z;
    return b;
}

Vec4f operator *(Mat4 m, Vec4f a){
    Vec4f b;
    b.x = m.arr[0][0] * a.x + m.arr[0][1] * a.y + m.arr[0][2] * a.z + m.arr[0][3] * a.w;
    b.y = m.arr[1][0] * a.x + m.arr[1][1] * a.y + m.arr[1][2] * a.z + m.arr[1][3] * a.w;
    b.z = m.arr[2][0] * a.x + m.arr[2][1] * a.y + m.arr[2][2] * a.z + m.arr[2][3] * a.w;
    b.w = m.arr[3][0] * a.x + m.arr[3][1] * a.y + m.arr[3][2] * a.z + m.arr[3][3] * a.w;
    return b;
}



Mat4 scaleAboutOrigin(float sx, float sy, float sz){
    return Mat4{
        sx,0,0,0,
        0,sy,0,0,
        0,0,sz,0,
        0,0,0,1
    };
}

Mat4 translate3D(float tx, float ty, float tz){
    return Mat4{
        1,0,0,tx,
        0,1,0,ty,
        0,0,1,tz,
        0,0,0,1
    };
}


Mat3 scaleAboutOrigin2D(float sx, float sy){
    return Mat3{
        sx,0,0,
        0,sy,0,
        0,0,1
    };
}

Mat3 translate2D(float tx, float ty){
    return Mat3{
        1,0,tx,
        0,1,ty,
        0,0,1
    };
}

Mat3 rotateAboutOrigin2D(float degrees){
    float radians = Radians(degrees);
    float cos = cosf(radians); 
    float sin = sinf(radians); 
    return Mat3{
        cos, -sin, 0,
        sin, cos, 0,
        0, 0, 1
    };
}


Mat3 rotateAboutPoint2D(float degrees, Vec2f pivot){
    Mat3 m = translate2D(-pivot.x, -pivot.y);
    m = rotateAboutOrigin2D(degrees) *m;
    m = translate2D(pivot.x, pivot.y) *m;
    return m;
}