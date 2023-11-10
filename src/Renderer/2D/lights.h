#pragma once

#include <math/math_er.h>


struct PointLight{
    Vec3f lightPos;
    Vec3f color;
    // ambient, diffuse and specular constants
    float ka, kd, ks, ns;
    // attenuation constants
    float k2, k1, k0;

    PointLight(){}
    // SET LIGHTPOS: all others can be defaulted
    PointLight(Vec3f pos, Vec3f color = Vec3f{1.0f,1.0f,1.0f}){
        this->color = color;
        lightPos = pos;

        kd = ks = 1.0f;
        ka = 0.21f;
        ns = 64;
        k1 = k2 = 0;
        k0 = 1;
    }


    Vec3f getIntensity(Vec3f pos, Vec3f normal, Vec3f view){
        Vec3f pToLight = lightPos - pos;
        float diffuse = kd * Clamp(0,dotProduct(pToLight, normal),1);
        
        Vec3f reflected = 2 * dotProduct(pToLight, normal) * normal - pToLight;
        float specular = ks * powf(Clamp(0, dotProduct(reflected, view), 1), ns);

        Vec3f result = Clamp(0, ka + diffuse + specular, 1.0f) * color;
        return result;
    }

    Vec3f getIntensity_attenuated(Vec3f pos, Vec3f normal, Vec3f view){
        Vec3f pToLight = normalize(lightPos - pos);
        float diffuse = kd * Clamp(0,dotProduct(pToLight, normal),1);
        
        Vec3f reflected = 2 * dotProduct(pToLight, normal) * normal - pToLight;
        float specular = ks * powf(Clamp(0, dotProduct(reflected, view), 1), ns);

        float d = len(lightPos - pos);

        // Vec3f result = Clamp(0, (specular)/(k2 * d*d + k1*d + k0), 1.0f) * color;
        Vec3f result = Clamp(0, ka + (diffuse + specular)/(k2 * d*d + k1*d + k0), 1.0f) * color;
        return result;
    }

};


struct DirectionalLight{
    Vec3f direction;
    Vec3f color;
    float ka, kd, ks, ns;

    DirectionalLight(){}
    // SET DIRECTION: all others can be defaulted
    DirectionalLight(Vec3f direction, Vec3f color = Vec3f{1.0f,1.0f,1.0f}){
        this->color = color;
        this->direction = direction;

        kd = ks = ka = 1.0f;
        ns = 64;
    }

    Vec3f getIntensity(Vec3f pos, Vec3f normal, Vec3f view){
        Vec3f pToLight =  -direction;
        float diffuse = kd * Clamp(0,dotProduct(pToLight, normal),1);
        
        Vec3f reflected = 2 * dotProduct(pToLight, normal) * normal - pToLight;
        float specular = ks * powf(Clamp(0, dotProduct(reflected, view), 1), ns);

        Vec3f result = Clamp(0, ka + diffuse + specular, 1.0f) * color;
        return result;
    }

    
};

struct Spotlight{
    Vec3f lightPos;
    Vec3f direction;
    float cutoffAngleDegree;
    
    Vec3f color;
    float ka, kd, ks, ns;

    // attenuation constants
    float k2, k1, k0;

    Spotlight(){}
    // SET LIGHTPOS: all others can be defaulted
    Spotlight(Vec3f pos, Vec3f dir = Vec3f{0,-1.0f,0}, Vec3f color = Vec3f{1.0f,1.0f,1.0f}){
        cutoffAngleDegree = 60.0f;
        this->color = color;
        direction = dir;
        lightPos = pos;
        
        kd = ks = ka = 1.0f;
        ns = 64;
        k1 = k2 = 0;
        k0 = 1;
    }


    Vec3f getIntensity(Vec3f pos, Vec3f normal, Vec3f view){
        Vec3f pToLight =  normalize(lightPos - pos);
        float diffuse = kd * Clamp(0,dotProduct(pToLight, normal),1);
        
        Vec3f reflected = 2 * dotProduct(pToLight, normal) * normal - pToLight;
        float specular = ks * powf(Clamp(0, dotProduct(reflected, view), 1), ns);

        float cutoff = Max(0, dotProduct(pToLight, -direction) - cosf(Radians(cutoffAngleDegree)));
        
        Vec3f result = Clamp(0, cutoff * (ka + diffuse + specular), 1.0f) * color;
        return result;
    }
    
    Vec3f getIntensity_attenuated(Vec3f pos, Vec3f normal, Vec3f view){
        Vec3f pToLight =  lightPos - pos;
        float diffuse = kd * Clamp(0,dotProduct(pToLight, normal),1);
        
        Vec3f reflected = 2 * dotProduct(pToLight, normal) * normal - pToLight;
        float specular = ks * powf(Clamp(0, dotProduct(reflected, view), 1), ns);

        float cutoff = Max(0, dotProduct(pToLight, -direction) - cosf(Radians(cutoffAngleDegree)));

        float d = len(lightPos - pos); 

        Vec3f result = Clamp(0, cutoff * (ka + (diffuse + specular)/(k2 * d*d + k1*d + k0)), 1.0f) * color;
        return result;
    }

    
};
