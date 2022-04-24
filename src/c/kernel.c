// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/shell.h"
#include "header/kernel.h"

int main() {
    makeInterrupt21();
    fillKernelMap();
    shell();
    
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
        case 0x2:
            readSector(BX, CX);
            break;
        case 0x3:
            writeSector(BX, CX);
            break;
        case 0x4:
            read(BX, CX);
            break;
        case 0x5:
            write(BX, CX);
            break;
        case 0x6:
            executeProgram(BX, CX);
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
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) *0x100; // CH
    sector = mod(sector_number, 18) + 1; // CL

    head = mod(div(sector_number, 18), 2) *0x100; // DH
    drive = 0x00; // DL

    interrupt(0x13, 0x0201, buffer, cylinder + sector, head + drive);
}

void writeSector(byte* buffer, int sector_number) {
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) *0x100; // CH
    sector = mod(sector_number, 18) + 1; // CL

    head = mod(div(sector_number, 18), 2) *0x100; // DH
    drive = 0x00; // DL

    interrupt(0x13, 0x0301, buffer, cylinder + sector, head + drive);
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

void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
    struct node_filesystem   node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct map_filesystem    map_fs_buffer;
    // Tambahkan tipe data yang dibutuhkan
    int i, j, idx_node, idx_sector, empty_space;
    byte sector_entry_buffer[16];
    // Masukkan filesystem dari storage ke memori
    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER+1);
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
    readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

    // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
    //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
    //    Jika ditemukan node yang cocok, tuliskan retcode 
    //    FS_W_FILE_ALREADY_EXIST dan keluar. 

    for (i=0;i<64;i++) {
        if(strcmp(node_fs_buffer.nodes[i].name, metadata->node_name) && node_fs_buffer.nodes[i].parent_node_index == metadata->parent_index) {
            if(node_fs_buffer.nodes[i].sector_entry_index==FS_NODE_S_IDX_FOLDER) {
                *return_code = FS_R_TYPE_IS_FOLDER;
            } else {
                *return_code = FS_W_FILE_ALREADY_EXIST;
            }
            return;
        }
    }
    // 2. Cari entri kosong pada filesystem node dan simpan indeks.
    //    Jika ada entry kosong, simpan indeks untuk penulisan.
    //    Jika tidak ada entry kosong, tuliskan FS_W_MAXIMUM_NODE_ENTRY
    //    dan keluar.
    for(i=0;i<64;i++) {
        // Entry filesystem node kosong jika nama node adalah string kosong (panjang string 0).
        if(strlen(node_fs_buffer.nodes[i].name)==0) {
            idx_node = i;
            break;
        }
    }
    if(i==64) {
        *return_code = FS_W_MAXIMUM_NODE_ENTRY;
        return;
    }

    // 3. Cek dan pastikan entry node pada indeks P adalah folder.
    //    Jika pada indeks tersebut adalah file atau entri kosong,
    //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.
    if(metadata->parent_index != FS_NODE_P_IDX_ROOT && 
        (node_fs_buffer.nodes[metadata->parent_index].sector_entry_index != FS_NODE_S_IDX_FOLDER 
        || strlen(metadata->node_name)==0)) {
        *return_code = FS_W_INVALID_FOLDER;
        return;
    }

    // 4. Dengan informasi metadata filesize, hitung sektor-sektor 
    //    yang masih kosong pada filesystem map. Setiap byte map mewakili 
    //    satu sektor sehingga setiap byte mewakili 512 bytes pada storage.
    //    Jika empty space tidak memenuhi, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika ukuran filesize melebihi 8192 bytes, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika tersedia empty space, lanjutkan langkah ke-5.
    empty_space = 0;
    for(i=0;i<256;i++) {
        if(map_fs_buffer.is_filled[i]==0x0) {
            empty_space++;
        }
    }
    if(empty_space*512 < metadata->filesize) {
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    }
    if(metadata->filesize>8192) {
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    }

    // 5. Cek pada filesystem sector apakah terdapat entry yang masih kosong.
    //    Jika ada entry kosong dan akan menulis file, simpan indeks untuk 
    //    penulisan.
    //    Jika tidak ada entry kosong dan akan menulis file, tuliskan
    //    FS_W_MAXIMUM_SECTOR_ENTRY dan keluar.
    //    Selain kondisi diatas, lanjutkan ke proses penulisan.
    for(i=0;i<32;i++) {
        // Entry filesystem sector kosong jika byte pertama (indeks 0) bernilai 0
        if(sector_fs_buffer.sector_list[i].sector_numbers[0]==0x0 && metadata->filesize > 0) {
            idx_sector = i;
            break;
        }
    }
    if(i==32 && metadata->filesize > 0) {
        *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
    }

    // Penulisan
    // 1. Tuliskan metadata nama dan byte P ke node pada memori buffer
    bounded_strcpy(node_fs_buffer.nodes[idx_node].name, metadata->node_name, 14);
    node_fs_buffer.nodes[idx_node].parent_node_index = metadata->parent_index;

    // 2. Jika menulis folder, tuliskan byte S dengan nilai 
    //    FS_NODE_S_IDX_FOLDER dan lompat ke langkah ke-8
    if(metadata->filesize==0x0) {
        node_fs_buffer.nodes[idx_node].sector_entry_index = FS_NODE_S_IDX_FOLDER;
    } else {
        // 3. Jika menulis file, tuliskan juga byte S sesuai indeks sector
        node_fs_buffer.nodes[idx_node].sector_entry_index = idx_sector;
        // 4. Persiapkan variabel j = 0 untuk iterator entry sector yang kosong
        j = 0;
        // 5. Persiapkan variabel buffer untuk entry sector kosong
        // 6. Lakukan iterasi berikut dengan kondisi perulangan 
                // (penulisan belum selesai && i = 0..255)
        //    1. Cek apakah map[i] telah terisi atau tidak
        //    2. Jika terisi, lanjutkan ke iterasi selanjutnya / continue
        //    3. Tandai map[i] terisi
        //    4. Ubah byte ke-j buffer entri sector dengan i
        //    5. Tambah nilai j dengan 1
        //    6. Lakukan writeSector() dengan file pointer buffer pada metadata 
        //       dan sektor tujuan i
        //    7. Jika ukuran file yang telah tertulis lebih besar atau sama dengan
        //       filesize pada metadata, penulisan selesai
        i=0;
        while (i < 256 && j*512 < metadata->filesize) {
            if (!map_fs_buffer.is_filled[i]) {
                map_fs_buffer.is_filled[i] = 0x1;
                sector_entry_buffer[j] = i;
                writeSector(&(metadata->buffer[j*512]), i);
                j++;
            }
            i++;
        }
        while (j<16){
            sector_entry_buffer[j] = 0x0;
            j++;
        }
        
        // 7. Lakukan update dengan memcpy() buffer entri sector dengan 
        //    buffer filesystem sector
        memcpy(sector_fs_buffer.sector_list[idx_sector].sector_numbers, sector_entry_buffer, 16);
    }
    // 8. Lakukan penulisan seluruh filesystem (map, node, sector) ke storage
    //    menggunakan writeSector() pada sektor yang sesuai
    writeSector(&(map_fs_buffer.is_filled[0]), FS_MAP_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER+1);
    writeSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);    
    // 9. Kembalikan retcode FS_SUCCESS
    *return_code = FS_SUCCESS;
}

void executeProgram(struct file_metadata *metadata, int segment) {
  enum fs_retcode fs_ret;
  byte buf[8192];
  
  metadata->buffer = buf;
  read(metadata, &fs_ret);
  if (fs_ret == FS_SUCCESS) {
    int i = 0;
    for (i = 0; i < 8192; i++) {
      if (i < metadata->filesize)
        putInMemory(segment, i, metadata->buffer[i]);
      else
        putInMemory(segment, i, 0x00);
    }
    launchProgram(segment);
  }
  else
    printString("exec: file not found\r\n");
}
