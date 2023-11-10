#include "./shaders.h"


er_Texture2D TextureShader::texture_TradeOffer = loadTexture("./textures/trade.jpg");


Vec4f TextureShader::ps(Point p){
    Vec4f sampled = texture_TradeOffer.sample_vec4(p.uv.x,p.uv.y);
    return(sampled);
    // return Vec4f(p.color, 1.0f);
}

Point TextureShader::vs(Point p){
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