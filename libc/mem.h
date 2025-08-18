#ifndef MEM_H
#define MEM_H

#include "../include/types.h"

void mempcpy(char *dest,const char *src,int num_bytes);
void memset(uint8_t *dest,uint8_t val,uint32_t len);

#endif
