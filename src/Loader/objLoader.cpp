#include "./lexer.h"
#include "./objLoader.h"

#define for_range(i, st, end) for(int i=st; i<end; i++)
#define vec3_expand(v)     (v.x),(v.y),(v.z)


void meshDump(const ObjMeshData &m){
    printf("Mesh: %s\n", m.name);
    // printf("Vertices\n");
    // for_range(i,0,m.vertices.size()){
    //     printf("(%0.3f, %0.3f, %0.3f)\n", m.vertices[i].x, m.vertices[i].y, m.vertices[i].z);
    // }
    // printf("Texture coords\n");
    // for_range(i,0,m.texCoords.size()){
    //     printf("(%0.3f, %0.3f)\n", m.texCoords[i].x, m.texCoords[i].y);
    // }
    // printf("Normals\n");
    // for_range(i,0,m.normals.size()){
    //     printf("(%0.3f, %0.3f, %0.3f)\n", m.normals[i].x, m.normals[i].y, m.normals[i].z);
    // }

    for_range(j, 0, m.renderInfo.size()){
        printf("Material: %s\n", m.renderInfo[j].materialName);
        printf("Vertices: %llu\n", m.renderInfo[j].vIndices.size());
        printf("Texcoords: %llu\n", m.renderInfo[j].tIndices.size());
        printf("Normals: %llu\n", m.renderInfo[j].nIndices.size());
    }

    
    printf("\n\n");
}


void MaterialDump(const Material &m){
    printf("Material: %s\n", m.name);

    printf("ns: %f \n", m.ns);
    printf("ka: %f, %f, %f\n", vec3_expand(m.ka));
    printf("kd: %f, %f, %f\n", vec3_expand(m.kd));
    printf("ks: %f, %f, %f\n", vec3_expand(m.ks));
    printf("ke: %f, %f, %f\n", vec3_expand(m.ke));
    printf("ni: %f\n", m.Ni);
    printf("\n\n");
}

Material loadMtl(Lexer &l){
    Material m;

    while (l.cursor < l.fileSize){
        DataString identifier = l.skipAndParseString();

        // ns - specular constant
        if (compare(identifier, "Ns")){
            float Ns = l.skipAndParseFloat();
            m.ns = Ns;
        }
        // ka - ambient color
        else if (compare(identifier, "Ka")){
            Vec3f Ka; 
            Ka.x = l.skipAndParseFloat();
            Ka.y = l.skipAndParseFloat();
            Ka.z = l.skipAndParseFloat();
            m.ka = Ka;
        }
        // kd - diffuse color
        else if (compare(identifier, "Kd")){
            Vec3f Kd; 
            Kd.x = l.skipAndParseFloat();
            Kd.y = l.skipAndParseFloat();
            Kd.z = l.skipAndParseFloat();
            m.kd = Kd;
        }
        // ks - specular color
        else if (compare(identifier, "Ks")){
            Vec3f Ks; 
            Ks.x = l.skipAndParseFloat();
            Ks.y = l.skipAndParseFloat();
            Ks.z = l.skipAndParseFloat();
            m.ks = Ks;
        }
        // d - opacity
        else if (compare(identifier, "d")){
            float d = l.skipAndParseFloat();
            m.d = d;
        }
        // Ni - refractive index
        else if (compare(identifier, "d")){
            float d = l.skipAndParseFloat();
            m.d = d;
        }
        else if (compare(identifier, "Illum")){
            int illum = l.skipAndParseInt();
        }
        else {
            l.revertToLineStart();
            break;
        }
    }
    return m;
}



MtlFileInfo loadMtlFile(const char *path){
    MtlFileInfo mtlFile;
    Lexer l;
    l.loadFile(path);
    
    while (l.cursor < l.fileSize){
        // skip commented line in .obj
        if(l.data[l.cursor] == '#'){
            l.skipCurrentLine();
            continue;
        }

        DataString identifier = l.skipAndParseString();

        // new mesh 
        if (compare(identifier, "newmtl")){
            DataString materialName = l.skipAndParseString();

            l.skipCurrentLine();
            Material m = loadMtl(l);
            
            copyToArray(materialName, m.name, sizeof(m.name)); 
            MaterialDump(m);
            
            mtlFile.materials.push_back(m);
        }
        
        l.skipCurrentLine();
    }

    return(mtlFile);
}





ObjMeshData loadMesh(Lexer &l, ObjFileInfo *f){
    ObjMeshData m;
    while (l.cursor < l.fileSize){
        DataString identifier = l.skipAndParseString();
        
        // vertex
        if (compare(identifier, "v")){
            Vec3f v;
            v.x = l.skipAndParseFloat();
            v.y = l.skipAndParseFloat();
            v.z = l.skipAndParseFloat();
            f->vertices.push_back(v);

            // update bounding box
            f->min.x = __min(f->min.x, v.x);
            f->min.y = __min(f->min.y, v.y);
            f->min.z = __min(f->min.z, v.z);
            f->max.x = __max(f->max.x, v.x);
            f->max.y = __max(f->max.y, v.y);
            f->max.z = __max(f->max.z, v.z);
        }
        // texture coordinates
        else if (compare(identifier, "vt")){
            Vec2f tCoords;
            tCoords.x = l.skipAndParseFloat();
            tCoords.y = l.skipAndParseFloat();
            f->texCoords.push_back(tCoords);
        }
        // vertex normal
        else if (compare(identifier, "vn")){
            Vec3f normal;
            normal.x = l.skipAndParseFloat();
            normal.y = l.skipAndParseFloat();
            normal.z = l.skipAndParseFloat();
            f->normals.push_back(normal);
        }
        // 
        else if (compare(identifier, "s")){

        }
        // use material 
        else if (compare(identifier, "usemtl")){
            ObjMeshData::RenderInfo info = {};
            DataString materialName = l.skipAndParseString();
            copyToArray(materialName, info.materialName, sizeof(info.materialName));

            for (int i=0; i<f->mtl.materials.size(); i++){
                if (strcmp(f->mtl.materials[i].name, info.materialName) == 0){
                    info.materialIndex = i;
                }
            }

            m.renderInfo.push_back(info);
        }
        // faces data (indices)
        else if (compare(identifier, "f")){
            for_range(i, 0, 3){
                int vIndex = l.skipAndParseInt()-1;
                m.renderInfo.back().vIndices.push_back(vIndex);

                if (l.data[l.cursor] == '/' && isNumber(l.data[l.cursor+1])){
                    int tIndex = l.skipAndParseInt()-1;
                    m.renderInfo.back().tIndices.push_back(tIndex);
                }
                else 
                    l.cursor++;

                if (l.data[l.cursor] == '/' && isNumber(l.data[l.cursor+1])){
                    int nIndex = l.skipAndParseInt()-1;
                    m.renderInfo.back().nIndices.push_back(nIndex);
                }
            }
        }
        else{
            l.revertToLineStart();
            break;
        }

        l.skipCurrentLine();
    }
    return(m);

}


ObjFileInfo loadObj(const char *path){
    ObjFileInfo f = {  };
    Lexer l;
    l.loadFile(path);

    while (l.cursor < l.fileSize){
        // skip commented line in .obj
        if(l.data[l.cursor] == '#'){
            l.skipCurrentLine();
            continue;
        }

        DataString identifier = l.skipAndParseString();

        // use material 
        if (compare(identifier, "mtllib")){
            DataString mtlFilePath = l.skipAndParseString();

            DataString relativeDir = newDataString(path);
            int endBackSlash = findCharFromBack(relativeDir, '\\') + 1;
            if (endBackSlash == 0) endBackSlash = findCharFromBack(relativeDir, '/') +1;


            copyToArray(relativeDir, f.mtlFile, sizeof(f.mtlFile));
            copyToArray(mtlFilePath, f.mtlFile + endBackSlash, sizeof(f.mtlFile) - endBackSlash);
            
            f.mtl = loadMtlFile(f.mtlFile);
        }
        // mesh name 
        else if (compare(identifier, "o")){
            DataString name = l.skipAndParseString();
            l.skipCurrentLine();
            ObjMeshData m = loadMesh(l, &f);
            copyToArray(name, m.name, sizeof(m.name));            
            meshDump(m);
            f.meshes.push_back(m);
            continue;
        }
        
        l.skipCurrentLine();
    }
    
    return(f);
}

