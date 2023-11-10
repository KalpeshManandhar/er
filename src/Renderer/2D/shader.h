#pragma once
#include "./primitives.h"

typedef Point (*VertexShader)(Point p);
typedef Vec4f (*PixelShader)(Point p);


struct Shader{
    // VertexShader vs;
    // PixelShader ps;
    
    
    Vec3f surfaceNormal;
    Mat4 model;
    Mat4 view;
    // static Mat4 projection;

    virtual Point vs(Point p);
    virtual Vec4f ps(Point p);
    
};

