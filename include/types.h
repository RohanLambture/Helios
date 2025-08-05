#ifndef TYPES_H
#define TYPES_H

typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;

#define lower_16(address) (uint16_t) ((address) & 0xFFFF)
#define higher_16(address) (uint16_t) (((address)>>16) & 0xFFFF)

#endif