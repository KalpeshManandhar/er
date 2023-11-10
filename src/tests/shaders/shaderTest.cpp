#include "./shaders.h"



Vec4f TestShader::ps(Point p){
    return(Vec4f(1.0,1.0,1.0,1.0f));
}

Point TestShader::vs(Point p){
    Vec4f p4(p.pos, 1.0f);
    const float znear = 2.0f, zfar = 1000.0f;
    const float fovx = 90, fovy = 90;

    //apply transformations
    Mat4 m = model;
    m = view * m;
    m = perspectiveProjection(fovx, fovy, znear, zfar) * m;

    p.pos = (m * p4).xyz_h();
    return p;
}
