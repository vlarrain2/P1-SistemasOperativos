#include "files/files.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    //Listar y bitmap
    os_mount(argv[1], atoi(argv[2]));
    os_ls();
    os_bitmap(0);

    //Modified y bitmap
    os_mount(argv[1], atoi(argv[2]));
    os_bitmap(0);

    //open_read
    os_mount(argv[1], atoi(argv[2]));
    osFile* file = os_open("nene.txt", 'r');

    unsigned char* buffer = malloc(684);
    os_read(file, buffer, 684);
    printf("BUFFER: %s\n", buffer);
    os_read(file, buffer, 526);
    printf("BUFFER: %s\n", buffer);
    free(buffer);
    os_close(file);

    //remove error
    //eliminar archivo que no existe
    os_mount(argv[1], atoi(argv[2]));
    os_rm("new_file.txt");
    os_ls();

    //open_write
    os_mount(argv[1], atoi(argv[2]));
    osFile* file = os_open("new_file.txt", 'w');
    os_close(file);
    os_ls();

    //read error
    //leer archivo que no existe
    os_mount(argv[1], atoi(argv[2]));
    osFile* file = os_open("noexisto.jpg", 'r');

    //remove
    os_mount(argv[1], atoi(argv[2]));
    os_rm("new_file.txt");
    os_ls();

    //wirte error
    //escribir archivo que existe
    os_mount(argv[1], atoi(argv[2]));
    osFile* file = os_open("nene.txt", 'w');
}