#include "../header/shell.h"
#include "../header/command.h"
#include "../header/kernel.h"
#include "../header/shell_helper.h"


void shell() {
  char input_buf[64];
  char path_str[128];
  char** args;
  int argc;
  byte current_dir = FS_NODE_P_IDX_ROOT;

  clearScreen();
  testWrite("test.txt", "ini isi test.txt");
  testWrite("real.txt", "ini isi real.txt");
  // printHeaderShell();
  while (1) {
    print("CURDIR: <");
    printHex(current_dir);
    print(">\n");
    print("OSThanos:");
    cwd(current_dir);
    print("$? ");
    input(input_buf);

    argc = splitStr(input_buf, args, ' ');

    // print("Your command is:\n");
    // describeCmd(args, argc);
    // print("\n");

    if (strcmp(args[0], "cd") && argc == 2) {
      cd(&current_dir, args[1]);
    } else if (strcmp(args[0], "mkdir") && argc == 2) {
      mkdir(current_dir, args[1]);
    } else if (strcmp(args[0], "ls") && argc == 1) {
      ls(current_dir);
    } else if (strcmp(args[0], "cat") && argc == 2) {
      cat(current_dir, args[1]);
    } else if (strcmp(args[0], "mv") && argc == 3) {
      mv(current_dir, args[1], args[2]);
    } else if (strcmp(args[0], "cp") && argc == 3) {
      cp(current_dir, args[1], args[2]);
    } else {
      print("Unknown command or wrong format\n");
    }
    print("\n");
  }
}
