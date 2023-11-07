#pragma once

#include <math/vec.h>
#include <vector>




struct Material{
    float ns, ka, kd, ks, ke, Ni, d;
};



struct MtlFileInfo{
    std::vector<Material> materials;
};



struct ObjMesh{
    char name[64];

    std::vector<Vec3f> vertices;
    std::vector<Vec2f> texCoords;
    std::vector<Vec3f> normals;

    size_t nVertices;
    size_t ntexCoords;
    size_t nNormals;

    struct RenderInfo{
        char materialName[64];

        std::vector<uint32_t> vIndices;
        std::vector<uint32_t> tIndices;
        std::vector<uint32_t> nIndices;
    };
    std::vector<RenderInfo> renderInfo;

};

struct ObjFileInfo{
    char mtlFile[64];
    std::vector<ObjMesh> meshes;
};

ObjFileInfo loadObj(const char *path);
