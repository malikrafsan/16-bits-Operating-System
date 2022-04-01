#include "../header/command.h"

void ls(byte curDir) {
  char buffer[1024];
  char *name;
  int i, j, idxName, count;

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  count = 0;
  for (i = 0; i < 64; i++) {
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

void cd(byte *parent, char *path) {
  char buffer[1024];
  int i;
  byte cur_dir;
  bool flag_temp;
  char *str = "1234567890";

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  cur_dir = *parent;
  if (strcmp(path, "..") == true) {
    *parent = buffer[cur_dir];
  } else {
    for (i = 0; i < 64; i++) {
      if (buffer[i * 16] == cur_dir &&
          buffer[i * 16 + 1] == FS_NODE_S_IDX_FOLDER &&
          strcmp(buffer + i * 16 + 2, path) == true) {
        // print("buffer[i * 16 + 2]: ");
        // print(&buffer[i * 16 + 2]);
        // print("\n");
        flag_temp = true;
        cur_dir = i * 16;
        // print("curdir: ");
        // print(&buffer[cur_dir + 2]);
        // print("\n");
        break;
      }
    }
    *parent = cur_dir;
  }
}

// void cd(char *name, byte *parent){
//   char buffer[1024];
//   int i;

//   handleInterrupt21(0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
//   handleInterrupt21(0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

//   for (i = 0; i < 64; i++) {
//     if (buffer[i * 16] == *parent) {
//       if (buffer[i * 16 + 1] == FS_NODE_S_IDX_FOLDER && strcmp(buffer + i *
//       16 + 2, name) == 1) {
//         printString("Directory found\n");
//         *parent = i;
//       }
//     }
//   }

//   printString("Directory not found\n");
// }