// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char *string = "Hai"; // Deklarasikan variabel pada awal scope
    int i = 0;

    clearScreen();
    for (i = 0; i < 3; i++) {
        byte warna = 0xD;
        putInMemory(0xB000, 0x8000 + 2*i, string[i]);
        putInMemory(0xB000, 0x8001 + 2*i, warna);
    }

    while (true);    
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    // switch (AX) {
    //     case 0x0:
    //         printString(BX);
    //         break;
    //     case 0x1:
    //         readString(BX);
    //         break;
    //     default:
    //         printString("Invalid Interrupt");
    // }
}

void clearScreen() {
    int x,y;
    
    for(y=0;y<25;y++) {
        for(x=0;x<80;x++) {
            byte putih = 0xF;
            putInMemory(0xB000, 0x8000+ (80*y+x)*2, '\0');
            putInMemory(0xB000, 0x8001+ (80*y+x)*2, putih);
        }
    }
    interrupt(0x10, 0x0200, 0, 0, 0); // set kursor ke x=0,y=0
}
