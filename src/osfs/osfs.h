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
void os_ls();
void os_bitmap(unsigned num);
int get_bitmaps_number(int partition);
void os_mbt();
