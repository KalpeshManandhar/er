#pragma once

#include <math/vec.h>

struct Point{
    Vec3f p;
    Vec3f color;
    Vec2f uv;
    Vec3f normal;
};


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



