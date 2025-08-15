#ifndef UTIL_H
#define UTIL_H

#include "../include/types.h"

void mempcpy(char *dest,const char *src,int num_bytes);
void memset(uint8_t *dest,uint8_t val,uint32_t len);
void int_to_ascii (int num, char *str);

#endif
