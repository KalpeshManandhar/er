#include <Loader/objLoader.h>


int loadermain(int argc, char **argv){
    if (argc < 2){
        printf("Usage: ./loader.exe <path to obj>");
        return -1;
    }
    loadObj(argv[1]);
    
    return 0;
}