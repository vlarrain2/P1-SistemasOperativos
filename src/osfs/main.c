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
    //os_open("nene.txt", 'r');
    //os_ls();
    //os_mbt();
    os_rm("nene.txt");
}