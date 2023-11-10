#pragma once

#include <math/vec.h>
#include <math/mat.h>

struct Point{
    Vec3f pos;
    Vec3f color;
    Vec2f uv;
    Vec3f normal;
    Vec3f worldPos;
};


struct Line3D{
    Vec3f start, end;
};


template <typename T>
union Triangle{
    struct {
        T a,b,c;
    };
    T points[3];
};


struct Sphere3D{
    Vec3f center;
    float radius;
};

struct Cube{
    Vec3f center;
    float radii[3];
    Vec3f axes[3];

    Cube(){
        center = Vec3f{0,0,0}; 
        radii[0] = radii[1] = radii[2] = 1.0f; 
        axes[0] = Preset::X_AXIS;
        axes[1] = Preset::Y_AXIS;
        axes[2] = Preset::Z_AXIS;
    }
};

typedef Triangle<Vec3f> Triangle3D;
typedef Triangle<Point> Triangle3DEx;



