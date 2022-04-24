#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "std_type.h"
#include "filesystem.h"
#include "utils.h"

void exec(struct file_metadata *metadata, int segment);

void exit(byte cur_dir, int next_program_segment);

#endif