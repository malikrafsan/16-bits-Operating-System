#ifndef __STRING_H__
#define __STRING_H__

#include "../header/std_type.h"

unsigned int strlen(char *string);
// Mengembalikan panjang suatu null terminated string

bool strcmp(char *s1, char *s2);
// Mengembalikan true jika string sama

void strcpy(char *dst, char *src);
// Melakukan penyalinan null terminated string

void bounded_strcpy(char *dst, char *src, int bound);
// Melakukan penyalinan null terminated string dengan batas

void strcat(char *dst, char *src);
// Melakukan penggabungan null terminated string

void clear(byte *ptr, unsigned int n);
// Mengosongkan byte array yang memiliki panjang n

int splitStr(char* str, char** res, char delimiter);

void clearStr(char* str);

#endif
