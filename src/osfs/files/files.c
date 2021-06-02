#include "files.h"
#include "../osfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

osFile* os_open(char* filename, char mode){
    if (mode == 'r'){
        if (os_exists(filename)){
            //creamos osfile y asignamos el nombre
            osFile* file = malloc(sizeof(osFile));
            file -> name = filename;

            //buscamos id_absoluto de la particion y su tamaño
            long int id_absoluto = find_partition(CURRENT_PARTITION);
            long int size_partition = find_partition_size(CURRENT_PARTITION);

            //apertura del disco
            FILE *disk = fopen(DISK_NAME, "rb");
            unsigned char *buffer;
            buffer = malloc(sizeof(char) * BLOCK_SIZE);

            //recibimos MBT
            fseek(disk, 0, SEEK_SET);
            fseek(disk, 1024 + BLOCK_SIZE * id_absoluto, SEEK_SET);
            fread(buffer, sizeof(char), BLOCK_SIZE, disk);

            //Vamos a comparar filename con los nombres de los archivos del directorio
            //para encontrar el que estamos buscando
            char entryFileName[29];
            int result;
            for (int i = 0; i < 64; i++)
            {
                printf("entrada %i\n", i);
                /*long int aux = buffer[i*32];
                printf("aux sin shift: %li\n", aux);
                aux >>= 31*8; //identificación de byte de validez
                printf("aux: %li\n", aux);*/
                if (buffer[i*32] ^ (0x00)) //si es archivo entramos

                {
                    for (int actual_char = 4; actual_char < 32; actual_char ++)
                    {
                        unsigned char byte = buffer[i * 32 + actual_char];
                        entryFileName[actual_char - 4] = byte;
                    }
                    printf("Nombre:\n");
                    printf("%s\n",entryFileName);
                    result = strcmp(filename, entryFileName); //ver si nombres coinciden
                    if (result == 0) //en el caso de que coincidan veo los datos del archivo
                    {   

                        long int bytes_id_relativo = buffer[i*32 + 1]; //obtencion de id relativo
                        for (int j = 1; j < 3; j++)
                        {
                            bytes_id_relativo <<= 8;
                            bytes_id_relativo += buffer[i*32 + 1 + j];
                        }
                        
                        printf("id relativo: %li\n", bytes_id_relativo);
                        bytes_id_relativo = 25841;
                        //la ubicacion del bloque indice es en 1024 + id absoluto + offset
                        file -> location = MBT_SIZE + BLOCK_SIZE*id_absoluto + BLOCK_SIZE*bytes_id_relativo;

                        printf("location: %li\n", file -> location);
                        
                        unsigned char *buffer_index;
                        buffer_index = malloc(sizeof(char) * BLOCK_SIZE);

                        fseek(disk, 0, SEEK_SET);
                        fseek(disk, file->location, SEEK_SET); //ubicacion en bloque indice del archivo
                        fread(buffer_index, sizeof(char), BLOCK_SIZE, disk);

                        long int bytes_file_size = buffer_index[0];

                        for (int j = 1; j < 5; j++)
                        {
                            bytes_file_size <<= 8;
                            bytes_file_size += buffer_index[j];
                        }
                        printf("\nsize: %li\n\n", bytes_file_size);
                        file -> size = bytes_file_size;
                        break;
                    }
                }  
            }
            fclose(disk);
            return file;
        }
        else{
            printf("NO EXISTE ARCHIVO\n");
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
int os_close(osFile* file_desc); //cerrar disco y hacer free (?)
int os_rm(char* filename); //cambiar el byte de validez a 0x00 en bloque directorio

int os_exists(char* filename);