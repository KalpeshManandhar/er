#include "./shaders.h"

Mat4 TestShader::model = identity<4>();


Vec4f TestShader::pixelShade(Point p){
    return(Vec4f(p.p.z, p.p.z, p.p.z,1.0f));
}

Point TestShader::vertexShade(Point p){
    Vec4f p4(p.p, 1.0f);
    const float znear = 2.0f, zfar = 1000.0f;
    const float fovx = 90, fovy = 90;

    //apply transformations
    Mat4 m = identity<4>(); 
    m = model;
    m = perspectiveProjection(fovx, fovy, znear, zfar) * m;

    p.p = (m * p4).xyz_h();
    return p;
}
