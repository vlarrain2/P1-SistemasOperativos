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

    unsigned char* buffer = malloc(684);
    os_read(file, buffer, 684);
    printf("BUFFER: %s\n", buffer);
    os_read(file, buffer, 526);
    printf("BUFFER: %s\n", buffer);
    free(buffer);
    os_close(file);
}