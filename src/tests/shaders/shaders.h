#pragma once

#include <Renderer/2D/shader.h>
#include <Renderer/2D/projection.h>
#include <Renderer/2D/texture.h>
#include <math/mat.h>

struct TestShader: public Shader{
    // globals/uniforms
    static float time;
    static Mat4 model;

    TestShader(){
        vs = vertexShade;
        ps = pixelShade;
        model = identity<4>();
    }

    static Vec4f pixelShade(Point p);
    static Point vertexShade(Point p);
    
};

struct TextureShader: public Shader{
    // globals/uniforms
    static Mat4 model;
    static Mat4 view;
    static er_Texture2D texture_TradeOffer;
    

    TextureShader(){
        vs = vertexShade;
        ps = pixelShade;
        model = identity<4>();
    }

    static Vec4f pixelShade(Point p);
    static Point vertexShade(Point p);
    
};


