#include "util.h"
#include "../drivers/screen.h"

void memcpy(char *dest, char *src, int num_bytes){
	if(!dest || !src || num_bytes <= 0) return;
	
	int index = 0;
	while(index < num_bytes){
		dest[index] = src[index];
		index++;
	}
}