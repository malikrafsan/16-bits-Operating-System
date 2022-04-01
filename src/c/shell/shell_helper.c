#include "../header/shell_helper.h"

void print(char *string) { interrupt(0x21, 0x0, string, 0, 0); }

void input(char *string) { interrupt(0x21, 0x1, string, 0, 0); }

void describeCmd(char **args, int argc) {
  int i;
  char *str = "0. ";

  for (i = 0; i < argc; i++) {
    str[0]++;
    print(str);
    print(args[i]);
    print("\n");
  }
  str[0] = '0';
}

void printHeaderShell() {
  char *string = "Tugas Besar OS Thanos\n";
  int i = 0;

  print(string);
  for (i = 0; i < strlen(string); i++) {
    byte warna = 0xD;
    putInMemory(0xB000, 0x8001 + 2 * i, warna);
  }
}

void print_fs_retcode(enum fs_retcode return_code) {
  switch (return_code) {
  case FS_UNKNOWN_ERROR:
    print("Unknown error\n");
    break;
  case FS_SUCCESS:
    print("Success\n");
    break;
  case FS_R_NODE_NOT_FOUND:
    print("Node not found\n");
    break;
  case FS_R_TYPE_IS_FOLDER:
    print("Type is folder\n");
    break;
  case FS_W_FILE_ALREADY_EXIST:
    print("Already exist\n");
    break;
  case FS_W_NOT_ENOUGH_STORAGE:
    print("Not enough storage\n");
    break;
  case FS_W_MAXIMUM_NODE_ENTRY:
    print("Maximum node entry\n");
    break;
  case FS_W_MAXIMUM_SECTOR_ENTRY:
    print("Maximum sector entry\n");
    break;
  case FS_W_INVALID_FOLDER:
    print("Invalid folder\n");
    break;
  default:
    print("Unknown error\n");
    break;
  }
}

void testWrite() {
  struct file_metadata metadata;
  enum fs_retcode return_code;

  metadata.buffer = "ini isi dari test.txt";
  metadata.node_name = "test.txt";
  metadata.parent_index = FS_NODE_P_IDX_ROOT;
  metadata.filesize = 512;

  print("----\n");
  interrupt(0x21, 0x5, &metadata, &return_code, 0);
  print("return code: ");
  print_fs_retcode(return_code);
  print("\n");
}

void cwd(byte cur_dir) {
  char buffer[1024];
  char* temp = "/";
  char* cur_name = "";

  if (cur_dir == FS_NODE_P_IDX_ROOT) {
    print("/");
  }

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  while (1) {
    if (cur_dir == FS_NODE_P_IDX_ROOT) {
      break;
    }

    bounded_strcpy(temp + 1, cur_dir + 2, 14);
    strcat(cur_name, temp);
    cur_dir = buffer[cur_dir];
    temp = "/";
    break;
  }

  print(cur_name);
}