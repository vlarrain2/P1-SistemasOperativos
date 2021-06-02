#include "osfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    os_mount(argv[1], atoi(argv[2]));
    // os_exists("nene.txt");

    // os_mbt();
    // find_space_new_partition(51);

    os_create_partition(6, 51);
    printf("Posición: %ld\n", find_partition(6));
    printf("Size: %ld\n", find_partition_size(6));
    
    os_mbt();
}

// 2: posicion: 50, porte: 123.456
// 3: posicion: 200.000, porte: 50.000
// 4: posicion: 250.100, porte: 40.000

// nuevo: porte: 60