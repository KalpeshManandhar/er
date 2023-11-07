#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#define isDelimiter(X)  ((X == '\n')||(X == '\t')||(X == '\r')||(X == ',')||(X == 0))
#define isAlphabet(X)   (((X >= 'a')&&(X <= 'z'))||((X >= 'A')&&(X <= 'Z')))
#define isNumber(X)     ((X >= '0') && (X <= '9'))


struct DataBuffer{
    uint8_t *data;
    size_t fileSize;
};

struct DataString{
    uint8_t *str;
    uint32_t len; 
};

static bool compare(DataString s, const char *str){
    for (int i=0; i<s.len; i++){
        if (str[i] == 0)
            return false;
        if (str[i] != s.str[i]){
            return false;
        }
    }
    return true;
}

static void copyToArray(DataString src, char dest[], int destSize){
    memcpy_s(dest, destSize-1,  (char *)src.str, src.len); 
    int end = (src.len < (destSize-1))?src.len:destSize-1;
    dest[end] = 0;
}


static DataBuffer loadFileToBuffer(const char *filepath);
static bool doesStrContainChar(const char *str, char ch){
    for (int i=0; i<strlen(str); i++){
        if (ch == str[i])
            return true;
    }
    return false;
}



struct Lexer{
    uint8_t *data;
    size_t cursor;
    size_t fileSize;
    

    Lexer(){data = 0; cursor = 0; fileSize = 0;}

    /* 
    ALL parse functions require that the cursor is at the start of the number/minus sign immediately followed by the number
    */

    void loadFile(const char *path){
        DataBuffer d = loadFileToBuffer(path);
        data = d.data;
        fileSize = d.fileSize;
    }
    
    float parseFloat(){
        int a = 0, divBy = 1;
        int decimalFlag = 0;
        int sign = 1;

        if (data[cursor] == '-'){
            sign = -1;
            cursor++;
        }
            
        while(!isDelimiter(data[cursor])){
            if (data[cursor] == '.'){
                decimalFlag = 1;
                cursor++;
            }
            if (isNumber(data[cursor])){
                if (decimalFlag)
                    divBy *= 10;
                a = a*10 + (data[cursor] - '0');
            }
            else
                break;
            cursor++;
        }
        return((float)(a * sign)/divBy);
    }

    uint32_t parseUInt(){
        uint32_t a = 0;
        while (isNumber(data[cursor])){
            a = a*10 + (data[cursor] - '0');
            cursor++;
        }
        return a;
    }


    int32_t parseInt(){
        int32_t a = 0;
        int32_t sign = 1;
        if (data[cursor] == '-'){
            sign = -1;
            cursor++;
        }
        while (isNumber(data[cursor])){
            a = a*10 + (data[cursor] - '0');
            cursor++;
        }
        return a*sign;
    }

    
    void skipCurrentLine(){
        while((cursor < fileSize) && (data[cursor] != '\n')){
            assert(cursor < fileSize);
            cursor++;
        }
        cursor++;
    }

    void skipSpaces(){
        while(data[cursor] == ' '){
            assert(cursor < fileSize);
            cursor++;
        }
    }

    void skipUntilNumeric(){
        while (!isNumber(data[cursor])){
            assert(cursor < fileSize);
            cursor++;
        }
    }
    
    void skipUntilAlphabet(){
        while (!isAlphabet(data[cursor])){
            assert(cursor < fileSize);
            cursor++;
        }
    }

    void skipUntil_Inc(char ch){
        while (data[cursor] == ch){
            assert(cursor < fileSize);
            cursor++;
        }
        cursor++;
    }

    DataString parseString(const char *delimiters = " \n\r"){
        DataString s = {0,0};
        s.str = &data[cursor];
        while (true){
            assert(cursor < fileSize);
            if (doesStrContainChar(delimiters, data[cursor]))
                break;
            s.len++;
            cursor++;
        }
        return s;
    }

    
    DataString skipAndParseString(const char *delimiters = " \n\r"){
        skipUntilAlphabet();
        return parseString();
    }



    float skipAndParseFloat(){
        skipSpaces();
        return parseFloat();
    }
    
    uint32_t skipAndParseUInt(){
        skipUntilNumeric();
        return parseUInt();
    }
    
    int32_t skipAndParseInt(){
        skipUntilNumeric();
        return parseInt();
    }

    void revertToLineStart(){
        while (data[cursor] != '\n'){
            cursor--;
        }
        cursor++;
    }

};



static DataBuffer loadFileToBuffer(const char *filepath){
    DataBuffer buffer = {0, 0};
    FILE * f = NULL;
    fopen_s(&f, filepath,"rb");
    if (!f){
        printf("No file");
        return(buffer);
    }
        
    fseek(f, 0, SEEK_END);
    buffer.fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer.data = (uint8_t*)malloc(sizeof(*buffer.data) * buffer.fileSize);
    fread(buffer.data, 1, buffer.fileSize, f);
    fclose(f);
    return(buffer);
}


