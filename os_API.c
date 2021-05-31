#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_API.h"

char* disco = "./simdiskformat.bin";
int particion;

int main(int argc, char** argv)
{
    os_open("name", 'r');
    return 0;
}

unsigned int int_to_int(unsigned int k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_int(k / 2)));
}

char* int_to_string(unsigned int k)
{
    static char string[8];
    for (int i = 7 - 1; i >= 0; i--)
    {
        string[i] = k % 10 ? '1' : '0';
        k = k / 10;
    }
    return string;
}

void os_open(char* filename, char mode){
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
            unsigned char buffer[1024];
            fdisk = fopen(disco, "r+b");
            fread(buffer, 1, 1024, fdisk);
            char* partition; // 1 bit paravalidez y 7 bits para identificador de particion
            char* directory_id = malloc(3 * sizeof(char*)); // 3 bytes 
            char* partition_blocks; // 4 bytes
            for (int i=0; i<1024; i += 8){
                partition = int_to_string(int_to_int(buffer[i]));
                printf("validez: %d\n", partition[0]);
                printf("partition id: ");
                for (int j = 1; j < 8; j++)
                {
                    printf("%d", partition[j]);
                }
                printf("\n");

                directory_id = int_to_string(int_to_int(buffer[i + 1]));
                strcat(directory_id, int_to_string(int_to_int(buffer[i + 2])));
                strcat(directory_id, int_to_string(int_to_int(buffer[i + 3])));
                printf("directory id: ");
                for (int j = 0; j < 24; j++)
                {
                    printf("%d", directory_id[j]);
                }
                printf("\n");
                partition_blocks = int_to_string(int_to_int(buffer[i + 4]));
                strcat(partition_blocks, int_to_string(int_to_int(buffer[i + 5])));
                strcat(partition_blocks, int_to_string(int_to_int(buffer[i + 6])));
                strcat(partition_blocks, int_to_string(int_to_int(buffer[i + 7])));
                printf("partition blocks: ");
                for (int j = 0; j < 32; j++)
                {
                    printf("%d", partition_blocks[j]);
                }
                printf("\n");
                printf("\n");

                
                //for (j=0; j<8;j++){
                    // recorrer cada entrada de la MBT
                    // si la entrada esigual a la particion, guardad id absoluto y relativo, break
                //}
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

int os_exists(char* filename)
{
    return 1;
}