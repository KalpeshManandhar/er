#include "./shaders.h"





Vec4f ObjRenderShader::ps(Point p){
    Vec3f view = normalize(cameraPos - p.worldPos);
    Vec3f result = {};
    for (int i=0; i<nPointLights; i++){
        result = result + pointLights[i].getIntensity_attenuated(p.worldPos, surfaceNormal, view);
    }
    return Vec4f(color * result, 1.0f);

    // return Vec4f(material.)
}

Point ObjRenderShader::vs(Point p){
    Vec4f p4(p.pos, 1.0f);
    const float znear = 2.0f, zfar = 1000.0f;
    const float fovx = 90, fovy = 60;

    //apply transformations
    Mat4 m = model;

    p.worldPos = (m * p4).xyz();
    m = view * m;
    m = perspectiveProjection(fovx, fovy, znear, zfar) * m;

    p.pos = (m * p4).xyz_h();
    return p;
}