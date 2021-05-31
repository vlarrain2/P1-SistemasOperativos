#include "osfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


void os_mount(char* diskname, int partition){
    BLOCK_SIZE = 2048;
    MBT_SIZE = 1024;
    CURRENT_PARTITION = partition;
    DISK_NAME = diskname;
}

long int find_partition(int id){
    FILE *file = fopen(DISK_NAME, "rb");
    unsigned char *buffer;
    buffer = malloc(sizeof(char) * MBT_SIZE);
    fseek(file, 0, SEEK_SET);
    fread(buffer, sizeof(char), MBT_SIZE, file);
    unsigned int id_byte = id + 128; //id entregado (7 bits) + bit de validacion (8vo bit)
    for (int i = 0; i < 128; i++) //Recorro MBT entrada a entrada
    {
        if ((id_byte ^ buffer[i*8]) == 0) // si el and entrega 1111...11
        {
            long int bytes_id_absoluto = buffer[i*8 + 1]; // 1111 1111
            for (int j = 1; j < 3; j++)
            {
                bytes_id_absoluto <<= 8; // 1111 1111 0000 0000
                bytes_id_absoluto += buffer[i*8 + 1 + j]; // 1111 1111 1111 1111
            }
            return bytes_id_absoluto;
        }
    }
    fclose(file);
    free(buffer);
    return 0;
}

long int find_partition_size(int id)
    {
    FILE *file = fopen(DISK_NAME, "rb");
    unsigned char *buffer;
    buffer = malloc(sizeof(char) * MBT_SIZE);
    fseek(file, 0, SEEK_SET);
    fread(buffer, sizeof(char), MBT_SIZE, file);
    unsigned int id_byte = id + 128; //id entregado (7 bits) + bit de validacion (8vo bit)
    for (int i = 0; i < 128; i++) //Recorro MBT entrada a entrada
    {
        if ((id_byte ^ buffer[i*8]) == 0) // si el and entrega 1111...11
        {
            long int bytes_id_absoluto = buffer[i*8 + 4]; // 1111 1111
            for (int j = 1; j < 4; j++)
            {
                bytes_id_absoluto <<= 8; // 1111 1111 0000 0000
                bytes_id_absoluto += buffer[i*8 + 4 + j]; // 1111 1111 1111 1111
            }
            return bytes_id_absoluto;
        }
    }
    fclose(file);
    free(buffer);
    return 0;
}

void os_ls(){
    FILE *file = fopen(DISK_NAME, "rb");
    unsigned char *buffer;
    buffer = malloc(sizeof(char) * BLOCK_SIZE);
    int address_block = MBT_SIZE + find_partition(CURRENT_PARTITION)*BLOCK_SIZE;
    printf("Files in Partition %d with block_adress: %d\n", CURRENT_PARTITION, address_block);
    fseek(file, address_block, SEEK_SET);
    fread(buffer, sizeof(char), BLOCK_SIZE, file);
    for (int i = 0; i < 64; i++) //64 es el número de entradas en un Bloque de Directorio
    {
        long int aux = buffer[i*32];
        aux >>= 24;

        if (aux ^ (0x01))
        {
            for (int actual_char = 4; actual_char < 32; actual_char ++ ) //se actualiza el fileName[29] byte a byte, espero que el string mismo sepa hasta dónde es según el null terminator
            {
              printf("%c", buffer[i*32 + actual_char]);
            }
            printf("\n");
        }
    }
    printf("\n");
    free(buffer);
    fclose(file);
}

int os_exists(char* filename)
{
    FILE* file = fopen(DISK_NAME, "rb");
    unsigned char* buffer;
    buffer = malloc(sizeof(char) * BLOCK_SIZE);
    int address_block = MBT_SIZE + find_partition(CURRENT_PARTITION) * BLOCK_SIZE;
    char entryFileName[29];
    int result;

    printf("Files in Partition %d with block_adress: %d\n", CURRENT_PARTITION, address_block);
    fseek(file, address_block, SEEK_SET);
    fread(buffer, sizeof(char), BLOCK_SIZE, file);
    for (int i = 0; i < 64; i++)
    {
        long int aux = buffer[i*32];
        aux >>= 24;

        if (aux ^ (0x01))
        {
            for (int actual_char = 4; actual_char < 32; actual_char ++)
            {
                unsigned char byte = buffer[i * 32 + actual_char];
                entryFileName[actual_char - 4] = byte;
            }
            result = strcmp(filename, entryFileName);
            if (result == 0)
            {
                fclose(file);
                free(buffer);
                printf("El archivo está en la partición \n");
                return(1);
            }
        }    
    }
    fclose(file);
    free(buffer);
    return(0);
}