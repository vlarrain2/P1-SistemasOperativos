#include "files.h"
#include "../osfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

void os_open(char* filename, char mode){
    if (mode == "r"){
        

    }
    else{

    }
};
int os_read(osFile* file_desc, void* buffer, int nbytes);
int os_write(osFile* file_desc, void* buffer, int nbytes);
int os_close(osFile* file_desc);
int os_rm(char* filename);

int os_exists(char* filename);