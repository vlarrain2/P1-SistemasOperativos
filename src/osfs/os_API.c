#include "os_API.h"
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
                if (byte !=0){
                printf("byte: %u ", byte);
                printf("byte shifted: %u\n", byte<<=1);
                }
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
    int valids = 0;
    for (int i =0; i<127;i++)
    {
        if (find_partition(i)!=0)
        {
            printf("Particion Valida de id %i\n", i);
            valids++;
        }
    }
    if (valids==0)
    {
        printf("No hay particiones validas\n");
    }
}


void os_ls()
{
    FILE *file = fopen(DISK_NAME, "rb");
    unsigned char *buffer;
    buffer = malloc(sizeof(char) * BLOCK_SIZE);
    int address_block = MBT_SIZE + find_partition(CURRENT_PARTITION)*BLOCK_SIZE;
    printf("Files in Partition %d with block_adress: %d\n", CURRENT_PARTITION, address_block);
    fseek(file, address_block, SEEK_SET);
    fread(buffer, sizeof(char), BLOCK_SIZE, file);
    for (int i = 0; i < 64; i++) //64 es el número de entradas en un Bloque de Directorio
    {
        if (buffer[i*32] ^ (0x00))
        {
            for (int actual_char = 4; actual_char < 32; actual_char ++ )
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

void os_reset_mbt()
{
    for (int i =0; i<127;i++)
    {
        if (find_partition(i)!=0)
        {
            os_delete_partition(i);
        }
    }
}

void os_delete_partition(int id){
    FILE *file = fopen(DISK_NAME, "r+b");
    unsigned char *buffer;
    buffer = malloc(sizeof(char) * MBT_SIZE);
    fseek(file, 0, SEEK_SET);
    fread(buffer, sizeof(char), MBT_SIZE, file);
    unsigned int id_byte = id + 128; //id entregado (7 bits) + bit de validacion (8vo bit)
    for (int i = 0; i < 128; i++) //Recorro MBT entrada a entrada
    {
        if ((id_byte ^ buffer[i*8]) == 0) // si el XOR entrega 0 es porque se encontro la entrada "i" con el id = id con el bit de validacion
        {
            fseek(file, i*8 , SEEK_SET);
            unsigned char id_invalido = (unsigned char) id;
            fwrite( &id_invalido, sizeof(id_invalido), 1, file);
            printf("Se ELIMINÓ la Partición %d \n", id);
        }
    }
    fclose(file);
    free(buffer);
}