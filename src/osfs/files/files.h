#pragma once

typedef struct osFile{
    char* name;
    long int location;
    long int size;
    long int directory_entry;
    char mode;
    long int bytes_processed;
}osFile;

osFile* os_open(char* filename, char mode);
int os_read(osFile* file_desc, void* buffer, int nbytes);
int os_write(osFile* file_desc, void* buffer, int nbytes);
int os_close(osFile* file_desc);
int os_rm(char* filename);

int os_exists(char* filename);

long int get_first_free_block();