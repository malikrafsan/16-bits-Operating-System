#ifndef __SHELL_HELPER__
#define __SHELL_HELPER__

#include "filesystem.h"
#include "std_lib.h"

void input(char *string);

void print(char *string);

// void describeCmd(char **args, int argc);

void printHeaderShell();

void testWrite(char* name, char* buffer);

void cwd(byte cur_dir);

void clearStr(char* str);

void printChar(char c);

void printHex(byte b);

byte getIdxDirByPath(byte cur_dir, char* path, bool *success);

void deleteFile(byte cur_dir, char* filename);

bool getParentPath(char *child, char *parent);

#endif
