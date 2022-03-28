// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char *str_input;
    char *string = "Haihaihai"; // Deklarasikan variabel pada awal scope
    int i = 0;

    fillKernelMap();
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

void fillKernelMap() {
    struct map_filesystem map_fs_buffer;
    int i;

    // Load filesystem map
    readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

    for(i=0;i<=15;i++) {
        map_fs_buffer.is_filled[i] = true;
    }
    for(i=256;i<=511;i++) {
        map_fs_buffer.is_filled[i] = true;
    }

    // Update filesystem map
    writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
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

void readSector(byte *buffer, int sector_number) {
    int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector = mod(sector_number, 18) + 1; // CL

    head = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00; // DL

    interrupt(0x13, 0x0200 | sector_read_count, buffer, cylinder | sector, head | drive);
}

void writeSector(byte* buffer, int sector_number) {
    int sector_write_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector = mod(sector_number, 18) + 1; // CL

    head = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00; // DL

    interrupt(0x13, 0x0300 | sector_write_count, buffer, cylinder | sector, head | drive);
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
    struct node_filesystem   node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    // Tambahkan tipe data yang dibutuhkan
    int sector_size = 512;
    int i, idx_sector, sector_number;
    struct sector_entry sector_entry_buffer;

    // Masukkan filesystem dari storage ke memori buffer
    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER+1);
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);

    // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
    //    Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
    //    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
    //    dan keluar.  

    for(i=0;i<64;i++) {
        if(strcmp(node_fs_buffer.nodes[i].name, metadata->node_name) && node_fs_buffer.nodes[i].parent_node_index == metadata->parent_index) {
            // 2. Cek tipe node yang ditemukan
            //    Jika tipe node adalah file, lakukan proses pembacaan.
            //    Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
            //    dan keluar.
            if(node_fs_buffer.nodes[i].sector_entry_index!=FS_NODE_S_IDX_FOLDER) {
                // Pembacaan
                // 1. memcpy() entry sector sesuai dengan byte S
                // 2. Lakukan iterasi proses berikut, i = 0..15
                // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
                // 4. Jika byte bernilai 0, selesaikan iterasi
                // 5. Jika byte valid, lakukan readSector() 
                //    dan masukkan kedalam buffer yang disediakan pada metadata
                // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
                // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.
                metadata->filesize = 0;
                idx_sector = node_fs_buffer.nodes[i].sector_entry_index;
                memcpy(&sector_entry_buffer, &(sector_fs_buffer.sector_list[idx_sector]), sizeof(struct sector_entry));
                for(i=0;i<16;i++) {
                    sector_number = sector_entry_buffer.sector_numbers[i];
                    if(sector_number==0x0) break;
                    if(sector_number>=16 && sector_number<=255) {
                        readSector(&(metadata->buffer[metadata->filesize]), sector_number);
                        metadata->filesize += sector_size;
                    }
                }
                metadata->buffer[metadata->filesize] = 0x0; // kasih null terminated
                *return_code = FS_SUCCESS;
                return;
            } else {
                *return_code = FS_R_TYPE_IS_FOLDER;
                return;
            }
        }
    }
    *return_code = FS_R_NODE_NOT_FOUND;
    return;
}
