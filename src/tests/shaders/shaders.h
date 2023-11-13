#pragma once

#include <Renderer/2D/shader.h>
#include <Renderer/2D/projection.h>
#include <Renderer/2D/texture.h>
#include <Renderer/2D/lights.h>

#include <math/mat.h>
#include <vector>

struct TestShader: public Shader{
    // globals/uniforms
    float time;

    TestShader(){
        view = model = identity<4>();
    }

    Vec4f ps(Point p) override;
    Point vs(Point p) override;
    
};

struct TextureShader: public Shader{
    // globals/uniforms
    static er_Texture2D texture_TradeOffer;
    

    TextureShader(){
        view = model = identity<4>();
    }

    Vec4f ps(Point p) override;
    Point vs(Point p) override;
    
};

struct LightingShader: public Shader{
    // globals/uniforms
    Vec3f cameraPos;
    PointLight *pointLights;
    int nPointLights;
    Spotlight *spotLights;
    int nspotLights;
    

    LightingShader(){
        view = model = identity<4>();
        cameraPos = {};
        pointLights = 0;
        nPointLights = 0;
        spotLights = 0;
        nspotLights= 0;
    }

    Vec4f ps(Point p) override;
    Point vs(Point p) override;
    
};


struct ObjRenderShader: public Shader{
    // globals/uniforms
    Vec3f cameraPos;
    Vec3f color;
    PointLight *pointLights;
    int nPointLights;
    Spotlight *spotLights;
    int nspotLights;
    

    ObjRenderShader(){
        view = model = identity<4>();
        color = {};
        cameraPos = {};
        pointLights = 0;
        nPointLights = 0;
        spotLights = 0;
        nspotLights= 0;
    }

    Vec4f ps(Point p) override;
    Point vs(Point p) override;
    
};


namespace Shaders{
    extern TestShader COLOR_SHADER;
    extern TextureShader TEXTURE_SHADER;
    extern LightingShader LIGHTING_SHADER;
};

