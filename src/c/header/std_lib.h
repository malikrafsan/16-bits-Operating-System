#ifndef __STD_LIB__
#define __STD_LIB__

#include "std_type.h"

// Operator matematika umum
// Implementasikan
// Tips : Gunakan C integer division
int div(int a, int b);

int mod(int a, int n);

// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, unsigned int n);
// Mengcopy n bytes yang ditunjuk src ke dest

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

#endif /* __STD_LIB__ */
