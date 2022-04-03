#include "../header/command.h"

void ls(byte curDir) {
  char buffer[1024];
  char name[15];
  int i, j, idxName;
  bool isEmpty;

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  // count = 0;
  isEmpty = true;
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
      // count++;
      isEmpty = false;
      clearStr(name);
    }
  }
  if (isEmpty) {
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
  print("return code: ");
  printHex(return_code);
  print("\n");
}

void cd(byte *parent, char *path) {
  char buffer[1024];
  byte res;
  bool success;

  res = getIdxDirByPath(*parent, path, &success);

  if (!success) {
    print("No such directory\n");
    return;
  }

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  if (res==FS_NODE_P_IDX_ROOT || buffer[res * 16 + 1] == FS_NODE_S_IDX_FOLDER) {
    *parent = res;
  } else {
    print("Not a directory\n");
  }
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
  print("\nreturn code: ");
  printHex(return_code);
  print("\n");
}

void mv(byte cur_dir, char *path_src, char *path_dest) {
  char buffer[1024];
  char **args;
  char *parent_dest;
  char *new_name;
  int argc, i;
  byte idx_src, idx_dest, idx_parent_dest;
  bool success_src, success_dest, success_parent_dest, success_get_parent;

  idx_src = getIdxDirByPath(cur_dir, path_src, &success_src);

  if (!success_src) {
    print("There is no ");
    print(path_src);
    print("\n");
    return;
  }

  idx_dest = getIdxDirByPath(cur_dir, path_dest, &success_dest);

  if (success_dest) {
    print(path_dest);
    print(" exist\n");
    return;
  }

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  if (buffer[idx_dest * 16 + 1] != FS_NODE_S_IDX_FOLDER &&
      idx_dest != FS_NODE_P_IDX_ROOT) {
    print("Cannot move to file\n");
    return;
  }

  success_get_parent = getParentPath(path_dest, parent_dest);

  // print("3. path_dest: ");
  // print(path_dest);
  // print("\n");

  if (!success_get_parent) {
    idx_parent_dest = FS_NODE_P_IDX_ROOT;
  } else {
    idx_parent_dest = getIdxDirByPath(cur_dir, parent_dest, &success_parent_dest);

    if (!success_parent_dest) {
      print("Unknown error\n");
      return;
    }
  }

  // print("4. path_dest: ");
  // print(path_dest);
  // print("\n");

  argc = splitStr(path_dest, args, '/');

  strcpy(new_name, args[argc - 1]);

  // print("new_name: ");
  // print(new_name);
  // print("\n");
  for (i=0;i < 14 && new_name[i] != '\0';i++) {
    buffer[idx_src * 16 + i + 2] = new_name[i];
  }
  for (;i < 14; i++) {
    buffer[idx_src * 16 + i + 2] = '\0';
  }

  buffer[idx_src * 16] = idx_parent_dest;

  interrupt(0x21, 0x3, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x3, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);
}

void cp(byte current_dir, char* src, char* dst) {
  struct file_metadata metadata;
  enum fs_retcode return_code;
  byte buftemp[16 * 512];
  int i;

  metadata.buffer = buftemp;
  metadata.node_name = src;
  metadata.parent_index = current_dir;

  // Copy buffer from src to metadata.buffer
  interrupt(0x21, 0x4, &metadata, &return_code, 0);

  metadata.node_name = dst;
  print("buffer: \n");
  print(metadata.buffer);
  print("\n");

  // jika tidak ada, maka create file
  interrupt(0x21, 0x5, &metadata, &return_code, 0);

  // jika file sudah ada, maka hapus file yang ada, lalu create file baru
  if (return_code == FS_W_FILE_ALREADY_EXIST) {
    deleteFile(current_dir, dst);
    interrupt(0x21, 0x5, &metadata, &return_code, 0);
  }
}