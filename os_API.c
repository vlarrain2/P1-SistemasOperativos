#include <stdio.h>
#include <stdlib.h>
#include "os_API.h"

char* disco;
int particion;

osFile* os_open(char* filename, char mode){
    if (mode == 'r'){
        // tiene que existir y luego lo representa
        if (os_exists(filename)){
            osFile* file = malloc(sizeof(osFile));
            // nombre archivo y donde está
            file -> name = filename;
            // primer bloque de la partición actual es el bloque de directorio
            // recorrer cada 32 bytes para saber si se hace mach con el filename
            // ultimos 28 bytes son el nombre
            FILE *fdisk;
            char buffer[1024];
            fdisk = fopen(disco, "r+b");
            fread(buffer, 1024, 128, fdisk);
            for (i=0; i<128; i++){
                for (j=0; j<8;j++){
                    // recorrer cada entrada de la MBT
                    // si la entrada esigual a la particion, guardad id absoluto y relativo, break
                }
            }
            // fseek(fdisk, 1024 + id_absoluto + offset_en_particion);
            // seleccionar los primeros 2KB (directorio)
            //recorrer el directorio en pedazos de 32B viendo los últimos 28 para el nombre de archivo

            //si hace mach con filename break
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