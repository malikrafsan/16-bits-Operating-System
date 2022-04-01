#include "../header/command.h"

void ls(byte curDir) {
  char buffer[1024];
  char* name;
  int i, j, idxName, count;

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  count = 0;
  for (i = 0; i< 64; i++) {
    if (buffer[i * 16] == curDir) {
      if (buffer[i * 16 + 1] == FS_NODE_S_IDX_FOLDER) {
        print("dir: ");
      } else {
        print("file: ");
      }

      j = 0;
      idxName = i * 16 + 2;
      bounded_strcpy(name, buffer + idxName, 14);
      print(name);
      print("\n");
      count++;
    }
  }
  if (count == 0) {
    print("Empty directory\n");
  }
}

void mkdir(byte cur_dir, char *name) {  
  struct file_metadata metadata;
  enum fs_retcode return_code;

  if (strlen(name) > 14) {
    print("Folder name too long\n");
    return;
  }

  metadata.buffer = 0x00;
  metadata.node_name = name;
  metadata.parent_index = cur_dir;
  metadata.filesize = 0;

  interrupt(0x21, 0x5, &metadata, &return_code, 0);
  print_fs_retcode(return_code);
}

void cd(byte* parent, char* path) {
  char buffer[1024];
  int i;
  byte cur_dir;
  bool flag_temp;

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  cur_dir = *parent;
  if (strcmp(path, "..") == true) {
    cur_dir = buffer[cur_dir];
  } else {
    for (i = 0; i < 64; i++) {
      if (buffer[i * 16] == cur_dir) {
        if (buffer[i * 16 + 1] == FS_NODE_S_IDX_FOLDER) {
          if (strcmp(buffer + i * 16 + 2, path) == true) {
            flag_temp = true;
            cur_dir = i;
            break;
          }
        }
      }
    }
  }
  if (flag_temp) {
    *parent = cur_dir;
    print("FOUND\n");
  } else {
    print("NOT FOUND\n");
  }
}

// void cd(char *name, byte *parent){
//   char buffer[1024];
//   int i;

//   handleInterrupt21(0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
//   handleInterrupt21(0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

//   for (i = 0; i < 64; i++) {
//     if (buffer[i * 16] == *parent) {
//       if (buffer[i * 16 + 1] == FS_NODE_S_IDX_FOLDER && strcmp(buffer + i * 16 + 2, name) == 1) {
//         printString("Directory found\n");
//         *parent = i;
//       }
//     }
//   }

//   printString("Directory not found\n");
// }