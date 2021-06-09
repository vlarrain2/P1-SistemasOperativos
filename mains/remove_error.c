#include "files/files.h"
#include "os_API.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, char **argv)
{
    //eliminar archivo que no existe
    os_mount(argv[1], atoi(argv[2]));
    os_rm("new_file.txt");
    os_ls();
}