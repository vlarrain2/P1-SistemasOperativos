#include "osfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    os_mount(argv[1], atoi(argv[2]));
    //long respuesta = find_partition(4);
    //long tamano = find_partition_size(4);
    os_bitmap(0);
}