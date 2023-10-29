#include "./texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


er_Texture2D loadTexture(const char * filepath){
    er_Texture2D tx;

    tx.texture.buffer = (uint32_t *)stbi_load(filepath, &tx.texture.w, &tx.texture.h, NULL, 4);
    if (!tx.texture.buffer){
        fprintf(stderr, "[ERROR] Cant open image: %s\n", filepath);
    }
    return tx;

}
