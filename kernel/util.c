#include "util.h"

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
void int_to_ascii(int num, char *str){
	int i = 0;
	int is_negative = 0;
	
	// Handle negative numbers
	if(num < 0){
		is_negative = 1;
		num = -num;
		str[i++] = '-';
	}
	
	// Handle zero case
	if(num == 0){
		str[i++] = '0';
		str[i] = '\0';
		return;
	}
	
	// Extract digits (they'll be in reverse order)
	int start = i;
	while(num > 0){
		str[i++] = num % 10 + '0';
		num /= 10;
	}
	str[i] = '\0';
	
	// Reverse the digits portion
	int end = i - 1;
	while(start < end){
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}
