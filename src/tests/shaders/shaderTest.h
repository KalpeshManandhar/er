#include <Renderer/2D/shader.h>
#include <math/mat.h>
#include <Renderer/2D/projection.h>


struct TestShader: public Shader{
    // globals/uniforms
    static float time;
    static Mat4 model;

    TestShader(){
        vs = vertexShade;
        ps = pixelShade;
        model = identity<4>();
    }

    static Vec4f pixelShade(Point p){
        return(Vec4f(p.color,1.0f));
    }
    
    static Point vertexShade(Point p){
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
};

Mat4 TestShader::model = identity<4>();


