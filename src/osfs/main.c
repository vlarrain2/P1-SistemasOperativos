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
    unsigned char* buffer = malloc(5);
    os_read(file, buffer, 2995);
    os_read(file, buffer, 526);
    //os_ls();
    //os_mbt();
    //os_rm("nene.txt");
}