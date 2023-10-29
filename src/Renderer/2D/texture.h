#pragma once

#include <stdint.h>
#include <Renderer/Buffer/er_buffer.h>
#include <math/interpolation.h>

struct er_Texture2D{
    er_Buffer2D texture;

    uint32_t sample(float u, float v){
        float x = u * texture.w;
        float y = v * texture.h;

        float t1 = x - (int)x;
        float t2 = y - (int)y;

        uint32_t a00 = texture.getValue((int)x, (int)y);
        uint32_t a01 = texture.getValue((int)x, (int)y+1);
        uint32_t a10 = texture.getValue((int)x+1, (int)y);
        uint32_t a11 = texture.getValue((int)x+1, (int)y+1);

        return bilerp(a00, a10, a11, a01, t1, t2);
    }
};


er_Texture2D loadTexture(const char * filepath);

