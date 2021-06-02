#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

int CURRENT_PARTITION;
char* DISK_NAME;
int BLOCK_SIZE;
int MBT_SIZE;




void os_mount(char* diskname, int partition);
long int find_partition(int id);
long int find_partition_size(int id);
long int find_invalid_partition();
void os_ls();
int os_exists(char* filename);
void os_bitmap(unsigned num);
int get_bitmaps_number(int partition);
void os_mbt();
void os_reset_mbt();
void os_delete_partition();
int find_space_new_partition(int size);
void os_create_partition(int id, int size);
