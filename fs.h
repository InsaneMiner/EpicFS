#ifndef __FS_H__
#define __FS_H__
#include "types.h"

struct file_nodes{
    char name[128];
    u32 permission;
    u32 length;  
};

struct FileObjStruct{
    u32 info;
    char name[128];
    u32 offset;
    u32 length;
    int currentOffset;
    int open;
};

typedef struct FileObjStruct FileObj;

u32 BLOCK = 512;

u32 SKIP_BLOCKS = 1;
#endif