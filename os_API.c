#include <stdio.h>
#include <stdlib.h>
#include "os_API.h"

osFile* os_open(char* filename, char mode){
    if (mode == 'r'){
        // tiene que existir y luego lo representa
        if (os_exists(filename)){
            osFile* file = malloc(sizeof(osFile));
            // nombre archivo y donde está
            file -> name = filename;
            file -> location; // averiguar si pueden haber direct anidados
        }
        else{
            printf("NO EXISTE ARCHIVO");
        }
    }
    else if (mode == 'w'){
        // verifica que no exista y crea uno nuevo
        if (os_exists(filename)){
            printf("YA EXISTE ARCHIVO");
        }
        else{
            osFile* file = malloc(sizeof(osFile));
            file -> name = filename;
            file -> location; //busca lugar libre para el archivo
        }
    }
}
int os_read(osFile* file_desc, void* buffer, int nbytes){

}
int os_write(osFile* file_desc, void* buffer, int nbytes){

}
int os_close(osFile* file_desc){

}
int os_rm(char* filename){

}