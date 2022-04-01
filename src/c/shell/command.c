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
      strcpy(name, buffer + idxName);
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