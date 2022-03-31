#include "../header/kernel.h"
#include "../header/std_lib.h"
#include "../header/filesystem.h"

void printHeaderShell() {
  char *string = "Tugas Besar OS Thanos\n";
  int i = 0;

  handleInterrupt21(0x0, string, 0, 0);
  for (i = 0; i < strlen(string); i++) {
    byte warna = 0xD;
    putInMemory(0xB000, 0x8001 + 2 * i, warna);
  }
}

void print(char *string) { handleInterrupt21(0x0, string, 0, 0); }

void input(char *string) { handleInterrupt21(0x1, string, 0, 0); }

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

void ls(byte curDir) {
  char buffer[1024];
  char name[14];
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
      while (j < 14 && buffer[idxName + j] != '\0') {
        // print(buffer[idxName + j]);
        name[j] = buffer[idxName + j];
        j++;
      }
      print(name);
      print("\r\n\0");
      name[0] = '\0';
      count++;
    }
  }
  if (count == 0) {
    print("Empty directory\n");
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
  enum fs_retcode return_code = 5;

  metadata.buffer = 0x00;
  metadata.node_name = "test.txt";
  metadata.parent_index = FS_NODE_P_IDX_ROOT;
  metadata.filesize = 0;
  
  print("----\n");
  handleInterrupt21(0x5, &metadata, &return_code, 0);
  print("return code: ");
  print_fs_retcode(return_code);
  print("\n");
}

void shell() {
  char input_buf[64];
  char path_str[128];
  char **args;
  int argc;
  byte current_dir = FS_NODE_P_IDX_ROOT;

  clearScreen();
  printHeaderShell();
  testWrite();
  while (1) {
    print("OSThanos:");
    print("/<path>"); // path
    print("$? ");
    input(input_buf);

    argc = splitStr(input_buf, args);

    print("Your command is:\n");
    describeCmd(args, argc);

    if (strcmp(args[0], "cd")) {
      if (argc != 2) {
        print("cd command must have 1 argument\n");
      } else {
        print("cd to ");
        print(args[1]);
        print("\n");
      }
    } else if (strcmp(args[0], "mkdir")) {
      if (argc != 2) {
        print("mkdir command must have 1 argument\n");
      } else {
        print("mkdir ");
        print(args[1]);
        print("\n");
      }
    } else if (strcmp(args[0], "ls")) {
      if (argc != 1) {
        print("ls command must have 0 argument\n");
      } else {
        print("listing directories and files\n");
        ls(current_dir);
      }
    } else {  
      print("Unknown command\n");
    }
  }
}
