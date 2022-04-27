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

int splitStr(char *str, char res[16][64], char delimiter) {
  int i = 0;
  int j = 0;
  int k = 0;
  int a = 0;
  char out[2];

  out[1] = '\0';

  // puts("\n<>\n");
  // puts(str);
  // puts("\n");

  while(str[i]==delimiter) {
    i++;
  }

  // puts("\n-----\n");
  while (str[i] != '\0') {
    if (str[i] == delimiter) {
      res[j][k] = '\0';
      
      while(str[i]==delimiter) {
        i++;
      }
      if (str[i] != '\0') {
        // puts("> ");
        // out[0] = '0';
        // puts(out);
        // puts(" : ");
        // puts(res[0]);
        // puts("\n");
        
        // puts("> ");
        // out[0] = '1';
        // puts(out);
        // puts(" : ");
        // puts(res[1]);
        // puts("\n");

        // puts("> ");
        // out[0] = '2';
        // puts(out);
        // puts(" : ");
        // puts(res[2]);
        // puts("\n");

        j++;
        k = 0;
      }
      i--;
    } else {
      res[j][k] = str[i];
      k++;
      // out[0] = str[i];
      // puts(out);
    }
    i++;
  }
  res[j][k] = '\0';
  // puts("> ");
  // out[0] = '0';
  // puts(out);
  // puts(" : ");
  // puts(res[0]);
  // puts("\n");

  // puts("> ");
  // out[0] = '1';
  // puts(out);
  // puts(" : ");
  // puts(res[1]);
  // puts("\n");

  // puts("> ");
  // out[0] = '2';
  // puts(out);
  // puts(" : ");
  // puts(res[2]);
  // puts("\n");
  
  // puts("-----\n");

  // puts("<>\n");
  // for (a=0; a<j+1; a++) {
  //   puts("{");
  //   out[0] = a + '0';
  //   puts(out);
  //   puts(" : ");
  //   puts(res[a]);
  //   puts("}\n");
  // }
  // puts("<>\n");

  return j + 1;
}

void clearStr(char* str) {
  int i;
  for (i = 0; str[i] != '\0'; i++) {
    str[i] = '\0';
  }
}