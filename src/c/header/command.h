#ifndef __COMMAND__
#define __COMMAND__

#include "std_type.h"
#include "shell_helper.h"

void cd(byte* cur_dir, char *path);
void ls(byte cur_dir);
void mkdir(byte cur_dir, char *name);
void mv(byte cur_dir, char *path_src, char *path_dest);
void cat(byte cur_dir, char *path);
void cp(byte cur_dir, char *path_src, char *path_dest);

#endif /* __COMMAND__ */
