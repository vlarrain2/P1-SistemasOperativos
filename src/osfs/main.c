#include "osfs.h"
#include "files/files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    os_mount(argv[1], atoi(argv[2]));
    osFile* file = os_open("nene.txt", 'r');
    printf("file location: %li\n", file -> location);
    unsigned char* buffer = malloc(3521);
    os_read(file, buffer, 3521);
    printf("BUFFER: %s\n", buffer);
    //os_read(file, buffer, 526);
    /*printf("name: %s\n", file -> name);
    printf("mode: %c\n", file -> mode);
    char buffer_write[] = "hola";
    long int write = os_write(file, buffer_write, 5);
    printf("ESCRIBI %li\n", write);*/
    //os_ls();
    //os_mbt();
    //os_rm("nene.txt");
    os_close(file);
}