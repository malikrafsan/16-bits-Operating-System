#include "../header/kernel.h"
#include "../header/std_lib.h"

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

void shell() {
  char input_buf[64];
  char *len;
  char **args;
  int argc;

  clearScreen();
  printHeaderShell();
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
      }
    } else {
      print("Unknown command\n");
    }
  }
}
