#pragma once
#include <stdint.h>
#include <assert.h>
#include <string.h>

#ifndef er_alloc
#include <stdlib.h>
#define er_alloc malloc
#define er_free free
#endif //er_ALLOCATOR


template <typename T>
struct er_BufferT{
    T *buffer;
    size_t w,h;

    er_BufferT(){}
    er_BufferT(size_t w, size_t h){
        buffer = NULL;
        resizeBuffer(w,h);
    }

    int resizeBuffer(size_t w, size_t h){
        if (buffer != NULL)
            er_free(buffer);
        buffer = (T *)er_alloc(w * h * sizeof(*buffer));
        if (buffer == NULL) return -1;
        this->w = w;
        this->h = h;
        return 0;
    }

    void setValue(int x, int y, T value){
        assert(0<=x && x<w);
        assert(0<=y && y<h);

        buffer[y * w + x] = value;
    }

    T getValue(int x, int y){
        assert(0<=x && x<w);
        assert(0<=y && y<h);

        return buffer[y * w + x];
    }
    

    void clearAll(T value){
        for (int i=0; i<w*h; i++)
            buffer[i] = value;
    }

    void clearAll_memset(uint8_t value){
        memset(buffer,value, w * h * sizeof(*buffer));
    }
};

typedef er_BufferT<uint32_t>    er_Buffer2D;
typedef er_BufferT<float>       er_Buffer2Df;
typedef er_BufferT<int>         er_Buffer2D_i32;



