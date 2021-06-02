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
            file -> mode = mode;
            file -> bytes_processed = 0;

            //buscamos id_absoluto de la particion y su tamaño
            long int id_absoluto = find_partition(CURRENT_PARTITION);
            long int size_partition = find_partition_size(CURRENT_PARTITION);

            //apertura del disco
            FILE *disk = fopen(DISK_NAME, "rb");
            unsigned char *buffer;
            buffer = malloc(sizeof(char) * BLOCK_SIZE);

            //recibimos directorio
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
                        file -> directory_entry =  MBT_SIZE + BLOCK_SIZE*id_absoluto + 32*i;
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
    else if (mode == 'w') {
        if (os_exists(filename)){
            printf("YA EXISTE ARCHIVO\n");
        }
        else{
            osFile* file = malloc(sizeof(osFile));
            file -> name = filename;
            file -> mode = mode;
            file -> bytes_processed = 0;

            FILE *disk = fopen(DISK_NAME, "r+b");
            unsigned char *buffer;
            buffer = malloc(sizeof(char) * BLOCK_SIZE);
            long int id_absoluto = find_partition(CURRENT_PARTITION);
            long int size_partition = find_partition_size(CURRENT_PARTITION);
            //recibimos MBT
            fseek(disk, 0, SEEK_SET);
            fseek(disk, 1024 + BLOCK_SIZE * id_absoluto, SEEK_SET);
            fread(buffer, sizeof(char), BLOCK_SIZE, disk);
            for (int i = 0; i < 64; i++)
            {
                if (buffer[i*32] ^ (0x01))
                {
                    fseek(disk, 0, SEEK_SET);
                    fseek(disk, MBT_SIZE + BLOCK_SIZE*id_absoluto + i*32, SEEK_SET);
                    unsigned char validez = 0x01;
                    fwrite(&validez, 1, 1, disk);
                    fseek(disk, 0, SEEK_SET);
                    fseek(disk, MBT_SIZE + BLOCK_SIZE*id_absoluto + BLOCK_SIZE, SEEK_SET);
                    long int id_relativo = 0;
                    int nofbitmaps = get_bitmaps_number(CURRENT_PARTITION);
                    for (int j=0; j<nofbitmaps; j++)
                    {
                        int bitMapPointer = find_partition(CURRENT_PARTITION)*BLOCK_SIZE + MBT_SIZE + (j+1)*BLOCK_SIZE;
                        buffer = malloc(sizeof(char) * BLOCK_SIZE);
                        fseek(disk, bitMapPointer, SEEK_SET);
                        fread(buffer, sizeof(char), BLOCK_SIZE, disk);
                        printf("Bitmap numero %i de la Particion %i\n", j+1, CURRENT_PARTITION);
                        for (int index = 0; index < BLOCK_SIZE; index++)
                        {
                            ///
                            unsigned int byte = buffer[index];
                            for (size_t k = 0; k < 8; k++)
                            {
                                unsigned int bit = byte & 0x080;
                                bit >>= 7;
                                if (bit == 0)
                                {
                                    id_relativo = j * BLOCK_SIZE * 8 + index*8 + k;
                                    fseek(disk, 0, SEEK_SET);
                                    fseek(disk, MBT_SIZE + id_absoluto*BLOCK_SIZE + BLOCK_SIZE + BLOCK_SIZE*j + index, SEEK_SET);
                                    int pos = 1;
                                    printf("pos sin shift: %i\n", pos);
                                    pos <<= 7 - k;
                                    printf("pos: %i\n", pos);
                                    printf("bitmap original: %i\n", buffer[index]);
                                    unsigned int new_byte = buffer[index] | pos;
                                    printf("bitmap modificado: %i\n", new_byte);
                                    fwrite(&new_byte, 1, 1, disk);
                                    break;
                                }
                                byte <<= 1;
                            }
                            if (id_relativo)
                            {
                                break;
                            }
                        }
                        if (id_relativo)
                        {
                            break;
                        }
                    }
                    printf("id relativo: %li\n", id_relativo);
                    printf("partition size: %li\n", size_partition);
                    fseek(disk, 0, SEEK_SET);
                    fseek(disk, MBT_SIZE + BLOCK_SIZE*id_absoluto + i*32 + 1, SEEK_SET);
                    fwrite(&id_relativo, 3, 1, disk);
                    fseek(disk, 0, SEEK_SET);
                    fseek(disk, MBT_SIZE + BLOCK_SIZE*id_absoluto + i*32 + 4, SEEK_SET);
                    fwrite(filename, 1, strlen(filename), disk);
                    char zero = 0x00;
                    for (int j = 0; j < 28 - strlen(filename); j++)
                    {
                        fseek(disk, 0, SEEK_SET);
                        fseek(disk, MBT_SIZE + BLOCK_SIZE*id_absoluto + i*32 + 4 + strlen(filename) + j, SEEK_SET);
                        fwrite(&zero, 1, 1, disk);
                    }
                    fclose(disk);
                    osFile* file = malloc(sizeof(osFile));
                    file -> name = filename;
                    file -> location = MBT_SIZE + id_absoluto*BLOCK_SIZE + id_relativo*BLOCK_SIZE;
                    file -> size = 0;
                    file -> directory_entry =  MBT_SIZE + BLOCK_SIZE*id_absoluto + 32*i;
                    return file;
                }
            }
        }
    }
};

int os_read(osFile* file_desc, void* buffer, int nbytes){
    if (file_desc -> mode != 'r'){
        printf("ERROR: NO HAS ABIERTO EL ARCHIVO PARA LEERLO\n");
        return 0;
    }

    else{
        long int id_absoluto = find_partition(CURRENT_PARTITION);
        // tamaño bytes - bytes leidos son los que quedan
        int result = file_desc->size - file_desc->bytes_processed;
        if (result >= nbytes){
            //se van a leer máximo nbytes, a menos que queden menos que nbytes por leer
            result = nbytes;
        }
        FILE *disk = fopen(DISK_NAME, "rb");
        fseek(disk, 0, SEEK_SET);
        long int blocks_read = (int)file_desc->bytes_processed/2048;
        printf("blocks read %li\n", blocks_read);

        long int bytes_in_block = file_desc->bytes_processed; //offset en el bloque para ubicacion
        if (blocks_read > 0){
            bytes_in_block = file_desc->bytes_processed%2048;
        }
        printf("bytes in block %li\n", bytes_in_block);

        fseek(disk, file_desc -> location + 5, SEEK_SET);

        unsigned char *index_block = malloc(BLOCK_SIZE - 5);
        int blocks = file_desc -> size / BLOCK_SIZE;
        fread(index_block, 1, BLOCK_SIZE - 5, disk);

        long int bytes_read = 0;
        char read_reasult[result];

        for (int i = blocks_read; i <= blocks; i++)
        {
            long int bytes_por_leer = 2048 - bytes_in_block;
            if (bytes_por_leer >= result - bytes_read){
                bytes_por_leer = result - bytes_read;
            }
            long int id_bloque = index_block[i*3];
            for (int j = 1; j < 3; j++)
            {
                id_bloque <<= 8;
                id_bloque += index_block[i*3 + j];
            }
            printf("id bloque %li\n", id_bloque);

            //ir al bloque de datos y leer byte a byte
            fseek(disk, 0, SEEK_SET);
            fseek(disk, MBT_SIZE + id_absoluto*BLOCK_SIZE + id_bloque*BLOCK_SIZE + bytes_in_block, SEEK_SET);
            
            unsigned char *buffer_read = malloc(sizeof(char) * result);
            
            fread(buffer_read, 1, bytes_por_leer, disk);
            for (int actual_char = 0; actual_char < bytes_por_leer; actual_char ++)
            {
                unsigned char byte = buffer_read[actual_char];
                read_reasult[bytes_read] = byte;
                bytes_read ++;
            }
            printf("reading: %li\n",bytes_read);
            if (bytes_read == result){
                break;
            }
        }
        printf("lectura: %s\n", read_reasult);
        file_desc->bytes_processed += result;


        return result;
    }
};


int os_write(osFile* file_desc, void* buffer, int nbytes){
    if (file_desc -> mode != 'w'){
        printf("ERROR: NO HAS ABIERTO EL ARCHIVO PARA ESCRIBIRLO\n");
        return 0;
    }
};
int os_close(osFile* file_desc); //cerrar disco y hacer free (?)

int os_rm(char* filename) //cambiar el byte de validez a 0x00 en bloque directorio
{
    if (os_exists(filename))
    {
        long int id_absoluto = find_partition(CURRENT_PARTITION);
        osFile* file = os_open(filename, 'r');
        FILE *disk = fopen(DISK_NAME, "r+b");
        int blocks = file -> size / BLOCK_SIZE;
        unsigned char *buffer;
        buffer = malloc(BLOCK_SIZE - 5);
        fseek(disk, 0, SEEK_SET);
        fseek(disk, file -> location + 5, SEEK_SET);
        fread(buffer, 1, BLOCK_SIZE - 5, disk);
        printf("blocks: %i\n", blocks);


        for (int i = 0; i <= blocks; i++)
        {
            long int id_bloque = buffer[i*3]; // 1111 1111
            for (int j = 1; j < 3; j++)
            {
                id_bloque <<= 8; // 1111 1111 0000 0000
                id_bloque += buffer[i*3 + j]; // 1111 1111 1111 1111
            }
            printf("id bloque: %li\n", id_bloque);

            int bitmap_block = (int)(id_bloque / (8*BLOCK_SIZE));
            int bitmap_pos = id_bloque % (8*BLOCK_SIZE);
            int bitmap_byte = (int)(bitmap_pos / 8);
            int bit = bitmap_pos % 8;
            printf("bitmap block: %i\n", bitmap_block);
            printf("bitmap pos: %i\n", bitmap_pos);
            printf("bitmap byte: %i\n", bitmap_byte);
            printf("bit: %i\n", bit);

            int bitMapPointer = id_absoluto*BLOCK_SIZE + MBT_SIZE + (bitmap_block+1)*BLOCK_SIZE;
            unsigned char *buffer_bitmap = malloc(sizeof(char) * BLOCK_SIZE);
            fseek(disk, bitMapPointer, SEEK_SET);
            fread(buffer_bitmap, sizeof(char), BLOCK_SIZE, disk);

            fseek(disk, 0, SEEK_SET);
            fseek(disk, MBT_SIZE + id_absoluto*BLOCK_SIZE + BLOCK_SIZE + BLOCK_SIZE*bitmap_block + bitmap_byte, SEEK_SET);
            unsigned int pos = 1;
            printf("pos sin shift: %ui\n", pos);
            pos <<= 7 - bit;
            pos = ~pos;
            printf("pos: %ui\n", pos);
            printf("bitmap original: %i\n", buffer_bitmap[bitmap_byte]);
            unsigned int new_byte = buffer_bitmap[bitmap_byte] & pos;
            printf("bitmap modificado: %i\n", new_byte);
            fwrite(&new_byte, 1, 1, disk);
        }

        fseek(disk, 0, SEEK_SET);
        fseek(disk, file -> directory_entry, SEEK_SET);
        unsigned char validez = 0x00;
        fwrite(&validez, 1, 1, disk);
        fclose(disk);
        return 0;
    }
    else
    {
        printf("ARCHIVO NO EXISTE\n");
        return 1;
    }
}