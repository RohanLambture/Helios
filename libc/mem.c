#include "mem.h"

void mempcpy(char *dest, const char *src, int num_bytes){
	int i = 0;
	while(i < num_bytes){
		dest[i] = src[i];
		i++;
	}
}

void memset(uint8_t *dest, uint8_t val, uint32_t len){
	uint8_t *temp = (uint8_t *) dest;
	while(len!=0){
		*temp = val;
		len--;
		temp++;
	}
}

