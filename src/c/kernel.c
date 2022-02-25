// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char *str_input;
    char *string = "Haihaihai"; // Deklarasikan variabel pada awal scope
    int i = 0;

    for (i = 0; i < 9; i++) {
        byte warna = 0xD;
        putInMemory(0xB000, 0x8000 + 2*i, string[i]);
        putInMemory(0xB000, 0x8001 + 2*i, warna);
    }

    handleInterrupt21(0x0, "Tugas Besar OS Thanos\n", 0, 0);
    handleInterrupt21(0x0, "Input new string: ", 0, 0);
    handleInterrupt21(0x1, str_input, 0, 0);
    handleInterrupt21(0x0, "print inputted string: ", 0, 0);
    handleInterrupt21(0x0, str_input, 0, 0);
    handleInterrupt21(0x0, "\nplease input anything before we clear screen\n", 0, 0);
    handleInterrupt21(0x1, str_input, 0, 0);

    clearScreen();

    handleInterrupt21(0x0, "screen has been clear up\n", 0, 0);

    while (true);    
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
        case 0x0:
            printString(BX);
            break;
        case 0x1:
            readString(BX);
            break;
        default:
            printString("Invalid Interrupt");
    }
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

void printString(char *string) {
    char *ret = '\r';
    char *new = '\n';

    while (*string != '\0') {
        if ((*string) == ret) {
            interrupt(0x10, 0xE00 + new, 0, 0, 0);
        } else if ((*string) == new) {
            interrupt(0x10, 0xE00 + ret, 0, 0, 0);
        }
        interrupt(0x10, 0xE00 + (*string), 0, 0, 0);
        string++;
    }
}

void readString(char *string) {
    int count = 0;
    int reading = 1;
    char back = '\b';
    char ret = '\r';

    while (reading) {
        char cin = interrupt(0x16, 0, 0, 0, 0);
        if (cin == ret) {
            interrupt(0x10, 0xe00 + '\r', 0, 0, 0);
            interrupt(0x10, 0xe00 + '\n', 0, 0, 0);
            (*string) = '\0';
            reading = 0;
        }
        else if (cin == back) {
            if (count > 0) {
                interrupt(0x10, 0xe00 + '\b', 0, 0, 0);
                interrupt(0x10, 0xe00 + '\0', 0, 0, 0);
                interrupt(0x10, 0xe00 + '\b', 0, 0, 0);
                (*string) = '\0';
                string--;
                count--;
            }
        }
        else {
            interrupt(0x10, 0xe00 + cin, 0, 0, 0);
            (*string) = cin;
            string++;
            count++;
        }
    }
}