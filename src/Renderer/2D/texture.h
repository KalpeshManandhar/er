#pragma once

#include <Renderer/Buffer/er_buffer.h>

#include <math/interpolation.h>
#include <math/utils.h>
#include <math/vec.h>
#include "./defines.h"



struct er_Texture2D{
    er_Buffer2D texture;
    enum class ColorOrder{
        U32_RGBA,
        U32_ARGB,
        U32_ABGR
    } hexOrder;

    uint32_t sample_u32(float u, float v){
        float x = u * (texture.w-1);
        float y = v * (texture.h-1);

        float t1 = x - (int)x;
        float t2 = y - (int)y;

        // nearest pixel
        return texture.getValue((int)Round(x)%texture.w, (int)Round(y)%texture.h);

        // doesnt work properly dunno why
        // interpolate from surrounding pixels
        uint32_t a00 = texture.getValue((int)x, (int)y);
        uint32_t a01 = texture.getValue((int)x, (int)(y+1)%texture.h);
        uint32_t a10 = texture.getValue((int)(x+1)%texture.w, (int)y);
        uint32_t a11 = texture.getValue((int)(x+1)%texture.w, (int)(y+1)%texture.h);

        uint8_t byte1 = bilerp(a00 & 0xff, a10 & 0xff, a11 & 0xff, a01 & 0xff, t1,t2);
        uint8_t byte2 = bilerp((a00>>8) & 0xff, (a10>>8) & 0xff, (a11>>8) & 0xff, (a01>>8) & 0xff, t1,t2);
        uint8_t byte3 = bilerp((a00>>16) & 0xff, (a10>>16) & 0xff, (a11>>16) & 0xff, (a01>>16) & 0xff, t1,t2);
        uint8_t byte4 = bilerp((a00>>24) & 0xff, (a10>>24) & 0xff, (a11>>24) & 0xff, (a01>>24) & 0xff, t1,t2);

        return uint32_t(byte1 | (byte2<<8) | (byte3<<16) | (byte4<<24));
    }


    Vec4f sample_vec4(float u, float v){
        uint32_t value = sample_u32(u,v);
        switch (hexOrder){
        case ColorOrder::U32_ABGR:
            return Vec4f{U32_ABGR_TO_VEC4(value)};
        case ColorOrder::U32_RGBA:
            return Vec4f{U32_RGBA_TO_VEC4(value)};
        case ColorOrder::U32_ARGB:
            return Vec4f{U32_ARGB_TO_VEC4(value)};
        default:
            break;
        }
        return Vec4f{U32_ABGR_TO_VEC4(value)};
    }   
};


er_Texture2D loadTexture(const char * filepath);

