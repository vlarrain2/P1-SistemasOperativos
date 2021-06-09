#include "files/files.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    os_mount(argv[1], atoi(argv[2]));
    osFile* file = os_open("nene.txt", 'r');

    // printf("file location: %li\n", file -> location);
    unsigned char* buffer = malloc(684);
    os_read(file, buffer, 684);
    printf("BUFFER: %s\n", buffer);
    os_read(file, buffer, 526);
    printf("BUFFER: %s\n", buffer);
    free(buffer);
    // os_ls();
    // os_bitmap(0);

    // osFile* file = os_open("new_file.txt", 'w');

    // os_bitmap(0);
    // printf("file location: %li\n", file -> location);
    // unsigned char* buffer = malloc(684);
    // os_read(file, buffer, 684);
    // printf("BUFFER: %s\n", buffer);
    // free(buffer);
    //os_read(file, buffer, 526);
    /*printf("name: %s\n", file -> name);
    printf("mode: %c\n", file -> mode);
    char buffer_write[] = "hola";
    long int write = os_write(file, buffer_write, 5);
    printf("ESCRIBI %li\n", write);*/
    //os_ls();
    //os_mbt();
    // os_rm("new_file.txt");
    // os_close(file);
    //os_mount(argv[1], atoi(argv[2]));
    //os_ls();
    //os_exists("nene.txt");
    //os_bitmap(0);
    //os_mbt();
    //os_delete_partition(4);
    //os_create_partition(6, 16384);
    //printf("Posición: %ld\n", find_partition(6));
    //printf("Size: %ld\n", find_partition_size(6));
    //os_create_partition(6,777777);
    //os_reset_mbt();
}