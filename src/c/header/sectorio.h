#ifndef __SECTORIO_H__
#define __SECTORIO_H__

#include "std_type.h"

void writeSectorLib(byte *buffer, int sector_number);

void readSectorLib(byte *buffer, int sector_number);

#endif