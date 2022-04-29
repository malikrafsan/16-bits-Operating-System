#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include "../header/filesystem.h"

void readFile(struct file_metadata *metadata, enum fs_retcode *return_code);

void writeLib(struct file_metadata *metadata, enum fs_retcode *return_code);

void deleteFile(byte cur_dir, char* filename);

byte getIdxByPath(byte cur_dir, char* path, bool *success);

bool getParentPath(char *child, char *parent);

#endif