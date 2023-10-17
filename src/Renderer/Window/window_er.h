#pragma once
#include <stdint.h>
#include <assert.h>
#include <string.h>

#ifndef er_alloc
#include <stdlib.h>
#define er_alloc malloc
#define er_free free
#endif //er_ALLOCATOR

struct er_Buffer{
    uint32_t *buffer;
    size_t w,h;

    er_Buffer(){}
    er_Buffer(size_t w, size_t h){
        buffer = NULL;
        resizeBuffer(w,h);
    }

    int resizeBuffer(size_t w, size_t h){
        if (buffer != NULL)
            er_free(buffer);
        buffer = (uint32_t *)er_alloc(w * h * sizeof(*buffer));
        if (buffer == NULL) return -1;
        this->w = w;
        this->h = h;
        return 0;
    }

    void setPixel(int x, int y, uint32_t value){
        assert(0<=x && x<w);
        assert(0<=y && y<h);

        buffer[y * w + x] = value;
    }

    uint32_t getPixel(int x, int y){
        assert(0<=x && x<w);
        assert(0<=y && y<h);

        return buffer[y * w + x];
    }
    

    void clearAll(uint32_t value){
        for (int i=0; i<w*h; i++)
            buffer[i] = value;
    }

    void clearAll_memset(uint8_t value){
        memset(buffer,value, w * h * sizeof(*buffer));
    }
};



