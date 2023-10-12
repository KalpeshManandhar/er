#pragma once

#include "../Window/window_er.h"
#include <math/vec.h>
#include "./defines.h"

struct er_Renderer3D{
    er_Buffer buffer;
    er_Buffer depthBuffer;

    er_Renderer3D(size_t w, size_t h){
        buffer = er_Buffer(w,h);
        depthBuffer = er_Buffer(w,h);
    }

};

struct Point{
    Vec3f p;
    Vec3f color;
};

int computeTriangle(er_Renderer3D *r, Point a, Point b, Point c);




// --------------------------------------------
//          PRIMITIVES/GEOMETRY - 3D
// --------------------------------------------
struct Line3D{
    Vec3f start, end;
};

struct Sphere3D{
    Vec3f center;
    float radius;
};

template <typename T>
union Triangle{
    struct {
        T a,b,c;
    };
    T points[3];
};

typedef Triangle<Vec3f> Triangle3D;
typedef Triangle<Point> Triangle3DEx;





