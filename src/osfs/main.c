#include "osfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    os_mount(argv[1], atoi(argv[2]));

    os_mbt();

}