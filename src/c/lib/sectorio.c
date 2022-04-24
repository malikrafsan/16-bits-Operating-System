#include "../header/sectorio.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void writeSectorLib(byte *buffer, int sector_number) {
    interrupt(0x21, 0x3, buffer, sector_number, 0x0);
}

void readSectorLib(byte *buffer, int sector_number) {
    interrupt(0x21, 0x2, buffer, sector_number, 0x0);
}
