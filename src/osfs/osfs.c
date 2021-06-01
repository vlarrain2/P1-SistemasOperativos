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


void os_bitmap(unsigned num){
    FILE *file;
	char *buffer;
	file = fopen(DISK_NAME, "rb");
    int freed = 0;
    int taken = 0;
    
    if (file == NULL)
	{
        exit(1);
	}
    if (num != 0)
    {
        ///definir puntero al bitmap que tenemos que imprimir
		int bitMapPointer = find_partition(CURRENT_PARTITION)*BLOCK_SIZE + MBT_SIZE + num*BLOCK_SIZE;
        buffer = malloc(sizeof(char) * BLOCK_SIZE);
		fseek(file, bitMapPointer, SEEK_SET);
		fread(buffer, sizeof(char), BLOCK_SIZE, file);
        printf("%i\n", bitMapPointer);
		for (int index = 0; index < BLOCK_SIZE; index++)
			{
				unsigned int byte = buffer[index];
				for (size_t i = 0; i < 8; i++)
				{
					//and con 10000000, si es 1 el valid bit es 1.
                    unsigned int bit = byte & 0x080;
					bit >>= 7;
					if (bit == 1)
						taken++;
					else
						freed++;
					byte <<= 1;
				}
                ///Printeamos dos digitos en hexadecimal (enunciado).
				printf("%02X", ((unsigned int)buffer[index]) & 0x0FF);
                ///Forma del print
				if (index % 16 == 15)
				{
					printf("\n");
				}
				else
				{
					printf(" ");
				}
			}
			printf("Bloques Ocupados: %i\n", taken);
			printf("Bloques Libres: %i\n", freed);
			printf("Total de Bloques: %i\n", freed + taken);
        free(buffer);
    }
    else if (num == 0)
    {
        ///Encontrar cuantos bloques bitmap tiene la particion (CURRENT_PARTITION)
        int nofbitmaps = get_bitmaps_number(CURRENT_PARTITION);
        for (int i=0; i<nofbitmaps; i++)
        {
            int bitMapPointer = find_partition(CURRENT_PARTITION)*BLOCK_SIZE + MBT_SIZE + (i+1)*BLOCK_SIZE;
            buffer = malloc(sizeof(char) * BLOCK_SIZE);
            fseek(file, bitMapPointer, SEEK_SET);
            fread(buffer, sizeof(char), BLOCK_SIZE, file);
            printf("Bitmap numero %i de la Particion %i\n", i+1, CURRENT_PARTITION);
            for (int index = 0; index < BLOCK_SIZE; index++)
			{
				///
				unsigned int byte = buffer[index];
				for (size_t i = 0; i < 8; i++)
				{
					unsigned int bit = byte & 0x080;
					bit >>= 7;
					if (bit == 1)
						taken++;
					else
						freed++;
					byte <<= 1;
				}
				///
				printf("%02X", ((unsigned int)buffer[index]) & 0x0FF);
				if (index % 16 == 15)
				{
					printf("\n");
				}
				else
				{
					printf(" ");
				}
			}
			printf("Bloques Ocupados: %i\n", taken);
			printf("Bloques Libres: %i\n", freed);
			printf("Total de Bloques: %i\n", freed + taken);
        free(buffer);
        } 
    }
    fclose(file);
}

int get_bitmaps_number(int partition)
{
	long int blocks = find_partition_size(partition);
	if (blocks%BLOCK_SIZE == 0)
	{
		return blocks/(BLOCK_SIZE*8);
	}
	else
	{
		return blocks/(BLOCK_SIZE*8) +1;
	}
} 

void os_mbt()
{
    for (int i =0; i<127;i++)
    {
        if (find_partition(i)!=0)
        {
            printf("Particion Valida de id %i\n", i);
        }
    }
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


void sort_partitions()
{
    int* sorted = malloc(sizeof(int) * 128);
    int count = 0;
    for (int h = 0; h < 128; h++)
    {
        sorted[h] = 999;
    } 


    for (int i = 0; i < 128; i++)
    {
        int min = 9999999;
        int id_min = 9999999;
        int index_min = 9999999;
        int used_i = 0;
        for (int k = 0; k < 128; k++)
        {
            if (i == sorted[k])
            {
                //printf("id: %d\n", i);
                used_i = 1;
            }
        }
        if (find_partition(i) != 0 && used_i == 0)
        {
            
            min = find_partition(i);
            id_min = i;
            index_min = i;
            for (int j = 0; j < 128; j++)
            {
                int used_j = 0;
                for (int z = 0; z < 128; z++)
                {
                    if (z != i)
                    {
                        if (j == sorted[z])
                        {
                            used_j = 1;
                        }
                    }
                }
                if (find_partition(j) != 0 && find_partition(j) < min && used_j == 0)
                {
                    min = find_partition(j);
                    id_min = j;
                    index_min = j;
                }
            }
        }
        if (min != 9999999)
        {
            sorted[count] = id_min;
            // mbt[index_min].is_valid = 0; Tengo que ver una forma de marvar com
            count++;
        }
    }
    for (int l = 0; l < count; l++)
    {
        printf("%d: %d en el byte %ld\n", l, sorted[l], find_partition(sorted[l]));
    }
    free(sorted);
}


void os_create_partition(int id, int size)
{
    printf("en proceso\n");
}