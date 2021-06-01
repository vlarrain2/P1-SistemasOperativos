#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "os_API.h"

char* disco = "./../simdiskfilled.bin";
int particion = 4;

int main(int argc, char** argv)
{
    os_open("name", 'r');
    return 0;
}

unsigned int int_to_int(unsigned int k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_int(k / 2)));
}

char* int_to_string(unsigned int k, int nbits)
{
    static char string[8];
    for (int i = (nbits - 1); i >= 0; i--)
    {
        string[i] = k % 10 ? '1' : '0';
        k = k / 10;
    }
    return string;
}

int string_to_int(int beg, int fin, char** binary, int nbytes)
{
    int exp = 0;
    int result = 0;
    for (int k=nbytes-1; k>=0; k--){
        for (int i=fin; i>=beg; i--){
            if (binary[k][i] == '1'){
                int potencia = 1;
                for (int j=0; j<exp;j++){
                    potencia *= 2;
                }
                result += potencia;
            }
            exp ++;
        }
    }
    return result;
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
            char** partition; // 1 bit paravalidez y 7 bits para identificador de particion
            char** directory_id = malloc(3 * sizeof(char*)); // 3 bytes 
            char** partition_blocks = malloc(4 * sizeof(char*)); // 4 bytes
            int contador = 0;
            for (int i=0; i<1024; i += 8){
                partition[0] = int_to_string(int_to_int(buffer[i]),8);
                printf("validez: %c\n", partition[0][0]);
                printf("partition int: %d\n", string_to_int(1,7,partition,1));
                printf("partition id: ");
                for (int j = 1; j < 8; j++)
                {
                    printf("%c", partition[0][j]);
                }
                printf("\n");

                if (partition[0][0] == '1'){
                    // int byte_directory = int_to_int(buffer[i + 1] + (buffer[i + 1]<<8) + (buffer[i + 1]<<16));
                    // directory_id = int_to_string(byte_directory,24);
                    directory_id[0] = int_to_string(int_to_int(buffer[i + 3]),8);
                    directory_id[1] = int_to_string(int_to_int(buffer[i + 2]),8);
                    directory_id[2] = int_to_string(int_to_int(buffer[i + 1]),8);
                    printf("directory int: %d\n", string_to_int(0,7,directory_id,3));
                    // strcat(directory_id, int_to_string(int_to_int(buffer[i + 2]),8));
                    // strcat(directory_id, int_to_string(int_to_int(buffer[i + 3]),8));
                    printf("directory id: ");
                    for (int j = 0; j < 3; j++)
                    {
                        printf("%s", directory_id[j]);
                    }
                    printf("\n");

                    partition_blocks[0] = int_to_string(int_to_int(buffer[i + 7]),8);
                    partition_blocks[1] = int_to_string(int_to_int(buffer[i + 6]),8);
                    partition_blocks[2] = int_to_string(int_to_int(buffer[i + 5]),8);
                    partition_blocks[3] = int_to_string(int_to_int(buffer[i + 4]),8);
                    printf("partition blocks int: %i\n", string_to_int(0,7,partition_blocks,4));
                    // partition_blocks = int_to_string(int_to_int(buffer[i + 4]),8);
                    // strcat(partition_blocks, int_to_string(int_to_int(buffer[i + 5]),8));
                    // strcat(partition_blocks, int_to_string(int_to_int(buffer[i + 6]),8));
                    // strcat(partition_blocks, int_to_string(int_to_int(buffer[i + 7]),8));
                    printf("partition blocks: ");
                    for (int j = 0; j < 4; j++)
                    {
                        printf("%s", partition_blocks[j]);
                    }
                    printf("\n");
                    printf("\n");
                }

                
                //for (j=0; j<8;j++){
                    // recorrer cada entrada de la MBT
                    // si la entrada esigual a la particion, guardad id absoluto y relativo, break
                //}
                printf("\n");
                printf("\n");
                contador ++;
            }
            printf("%i\n", contador);

            fseek(fdisk, 1024 + string_to_int(0,7,directory_id,3), SEEK_SET);
            unsigned char directory_buffer[2048];
            fread(directory_buffer, 1, 2048, fdisk);
            for (int i=0; i<2048; i++){
                printf("%c",directory_buffer[i]);
            }
            printf("\n");
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