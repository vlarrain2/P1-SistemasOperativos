#include "files.h"
#include "../osfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

void os_open(char* filename, char mode){
    if (mode == 'r'){
        if (1){
            osFile* file = malloc(sizeof(osFile));
            file -> name = filename;
            long int id_absoluto = find_partition(CURRENT_PARTITION);
            long int size_partition = find_partition_size(CURRENT_PARTITION);
            FILE *disk = fopen(DISK_NAME, "rb");
            unsigned char *buffer;
            buffer = malloc(sizeof(char) * BLOCK_SIZE);
            fseek(disk, 1024 + id_absoluto, SEEK_SET);
            fread(buffer, sizeof(char), BLOCK_SIZE, disk);
            printf("hola\n");
            for (int i = 0; i < 64; i++) //Recorro MBT entrada a entrada
            {
                long int bytes_directorio = buffer[i*8];
                for (int j = 1; j < 32; j++)
                {
                    bytes_directorio <<= 8; // 1111 1111 0000 0000
                    bytes_directorio += buffer[i*8 + j]; // 1111 1111 1111 1111
                }
                printf("%li\n",bytes_directorio);
                // if ((CURRENT_PARTITION ^ buffer[i*8]) == 0) // si el and entrega 1111...11
                // {
                //     long int bytes_id_absoluto = buffer[i*8 + 1]; // 1111 1111
                //     for (int j = 1; j < 3; j++)
                //     {
                //         bytes_id_absoluto <<= 8; // 1111 1111 0000 0000
                //         bytes_id_absoluto += buffer[i*8 + 1 + j]; // 1111 1111 1111 1111
                //     }
                //     return bytes_id_absoluto;
                // }
            }
            file -> location; //busca lugar libre para el archivo

            fclose(disk);
            
        }
        else{
            printf("NO EXISTE ARCHIVO");
        }

    }
    else{
        if (os_exists(filename)){
            printf("YA EXISTE ARCHIVO");
        }
        else{
            osFile* file = malloc(sizeof(osFile));
            file -> name = filename;
            file -> location; //busca lugar libre para el archivo
        }

    }
};
int os_read(osFile* file_desc, void* buffer, int nbytes);
int os_write(osFile* file_desc, void* buffer, int nbytes);
int os_close(osFile* file_desc);
int os_rm(char* filename);

int os_exists(char* filename);