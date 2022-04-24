#include "../header/string.h"
#include "../header/std_type.h"

unsigned int strlen(char *string) {
  int i = 0;
  while (string[i] != '\0') {
    i++;
  }
  return i;
}

bool strcmp(char *s1, char *s2) {
  int i;
  if (strlen(s1) != strlen(s2)) {
    return false;
  }

  for (i = 0; i < strlen(s1); i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

void strcpy(char *dst, char *src) {
  int i = 0;
  while (src[i] != '\0') {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';
}

void bounded_strcpy(char *dst, char *src, int bound) {
  int i = 0;
  while (src[i] != '\0' && i < bound) {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';
}

void strcat(char *dst, char *src) {
  int i, j;

  for (i = 0; dst[i] != '\0'; i++)
    ;
  for (j = 0; src[j] != '\0'; j++) {
    dst[i + j] = src[j];
  }
  dst[i + j] = '\0';
}

void clear(byte *ptr, unsigned int n) {
  int i;
  for (i = 0; i < n; i++) {
    ptr[i] = 0;
  }
}

int splitStr(char *str, char **res, char delimiter) {
  int i = 0;
  int j = 0;
  int k = 0;

  while(str[i]==delimiter) {
    i++;
  }

  while (str[i] != '\0') {
    if (str[i] == delimiter) {
      res[j][k] = '\0';
      j++;
      k = 0;
      while(str[i]==delimiter) {
        i++;
      }
      i--;
    } else {
      res[j][k] = str[i];
      k++;
    }
    i++;
  }
  res[j][k] = '\0';

  return j + 1;
}