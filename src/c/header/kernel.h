// Kernel header

#ifndef __KERNEL__
#define __KERNEL__

#include "std_type.h"
#include "std_lib.h"
#include "filesystem.h"
#include "shell_lib.h"

// Fungsi bawaan
extern void putInMemory(int segment, int address, byte b);
extern int interrupt (int int_number, int AX, int BX, int CX, int DX);
extern void makeInterrupt21();

void handleInterrupt21(int AX, int BX, int CX, int DX);
void fillKernelMap();

// Implementasikan
void printString(char *string);
void readString(char *string);
void clearScreen();

void writeSector(byte *buffer, int sector_number);
void readSector(byte *buffer, int sector_number);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);

extern void launchProgram(int segment);
void executeProgram(struct file_metadata *metadata, int segment);

#endif /* __KERNEL__ */
