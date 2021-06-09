#include "files/files.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    //leer archivo que no existe
    os_mount(argv[1], atoi(argv[2]));
    osFile* file = os_open("noexisto.jpg", 'r');
}