#ifndef __TEXT_IO_H__
#define __TEXT_IO_H__ 

#include "../header/utils.h"
#include "../header/filesystem.h"

int puts(char *string);

int gets(char *string);

int putsIntFlipped(int x);

int putsMsg(struct message* msg);

void puts_fs_retcode(enum fs_retcode return_code);

void putsHex(byte b);

void printHeader();

#endif /* __TEXT_IO_H__ */
