#include "../header/command.h"

void ls(byte curDir) {
  char buffer[1024];
  char name[15];
  int i, j, idxName, count;

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  count = 0;
  for (i = 0; i < 64; i++) {
    if (buffer[i * 16] == curDir && strlen(buffer + i * 16 + 2) != 0) {
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
      clearStr(name);
    }
  }
  if (count == 0) {
    print("Empty directory\n");
  }
  clearStr(buffer);
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
  *parent = getIdxByPath(*parent, path);
}

void cat(byte cur_dir, char *path) {
  struct file_metadata metadata;
  enum fs_retcode return_code;
  byte buftemp[16 * 512];

  metadata.buffer = buftemp;
  metadata.node_name = path;
  metadata.parent_index = cur_dir;

  interrupt(0x21, 0x4, &metadata, &return_code, 0);

  print("buffer: \n");
  print(metadata.buffer);
  print("\n");
  print_fs_retcode(return_code);
}
