#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    os_mount(argv[1], atoi(argv[2]));
    //os_mbt();
    //os_reset_mbt();
    //os_mbt();
    //os_bitmap(1);
    //os_ls();
    //os_create_partition(6, 16384);
    //printf("Posición: %ld\n", find_partition(6));
    //printf("Size: %ld\n", find_partition_size(6));
    //os_mbt();

}