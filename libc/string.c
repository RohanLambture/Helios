#include "string.h"

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

int strlen(const char s[]){
	int i=0;
	while(s[i]!='\0')i++;
	return i;
}
void append(char s[], char n) {
	int len = strlen(s);
	s[len] = n;
	s[len+1] = '\0';
}

void backspace(char s[]) {
	int len = strlen(s);
	if(len == 0)return;
	s[len-1] = '\0';
}

int strcmp(char s1[], char s2[]) {
	int i;
	for (i = 0; s1[i] == s2[i]; i++) {
		if (s1[i] == '\0') return 0;
	}
	return s1[i] - s2[i];
}
