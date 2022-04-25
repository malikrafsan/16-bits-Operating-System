#ifndef __UTILS_H__
#define __UTILS_H__

#include "std_type.h"
#include "sectorio.h"
#include "string.h"

extern int getCurrentSegment();

struct message {
    byte current_directory;
    char name[16];
    bool isLocalProgram;
    char arg1[64];
    char arg2[64];
    char arg3[64];
    int next_program_segment; // Dapat digunakan untuk bagian 3.5
    char other[300];
};

void getMessage(struct message *msg, int program_segment);

void getCurMessage(struct message *msg);

void set_MultiMsg(char* input_buf, byte curdir);

void setMessage(struct message *msg, int program_segment);

#endif