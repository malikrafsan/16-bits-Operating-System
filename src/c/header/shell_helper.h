#ifndef __SHELL_HELPER__
#define __SHELL_HELPER__

#include "filesystem.h"
#include "std_lib.h"

void printHeaderShell();

void input(char *string);

void print(char *string);

// void describeCmd(char **args, int argc);

// void printHeaderShell();

// void testWrite(char* name, char* buffer);

void print_fs_retcode(enum fs_retcode return_code);

void cwd(byte cur_dir);

void clearStr(char* str);

void printHex(byte b);

byte getIdxByPath(byte cur_dir, char* path, bool *success);

void deleteFile(byte cur_dir, char* filename);

bool getParentPath(char *child, char *parent);

#endif
