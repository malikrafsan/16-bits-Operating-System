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
  case -1:
    print("unknown error\n");
    break;
  case 0:
    print("success\n");
    break;
  case 1:
    print("node not found\n");
    break;
  case 2:
    print("type is folder\n");
    break;
  case 3:
    print("file already exist\n");
    break;
  case 4:
    print("not enough storage\n");
    break;
  case 5:
    print("maximum node entry\n");
    break;
  case 6:
    print("maximum sector entry\n");
    break;
  case 7:
    print("invalid folder\n");
    break;
  default:
    print("unknown error\n");
    break;
  }
}

void testWrite() {
  struct file_metadata metadata;
  enum fs_retcode return_code;

  metadata.buffer = 0x00;
  metadata.node_name = "test.txt";
  metadata.parent_index = FS_NODE_P_IDX_ROOT;
  metadata.filesize = 0;

  print("----\n");
  interrupt(0x21, 0x5, &metadata, &return_code, 0);
  print("return code: ");
  print_fs_retcode(return_code);
  print("\n");
}
