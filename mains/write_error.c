#include "files/files.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    //escribir archivo que existe
    os_mount(argv[1], atoi(argv[2]));
    osFile* file = os_open("nene.txt", 'w');
}