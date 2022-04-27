#ifndef __SHELL_LIB_H__
#define __SHELL_LIB_H__

#include "./std_type.h"

byte getIdxByPath(byte cur_dir, char* path, bool *success);

void puts_fs_retcode(enum fs_retcode return_code);

#endif /* __SHELL_LIB_H__ */