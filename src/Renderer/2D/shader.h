#pragma once
#include "./primitives.h"

typedef Point (*VertexShader)(Point p);
typedef Vec4f (*PixelShader)(Point p);


struct Shader{
    VertexShader vs;
    PixelShader ps;    
};