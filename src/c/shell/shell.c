#include "../header/kernel.h"
#include "../header/shell.h"
#include "../header/command.h"
#include "../header/shell_helper.h"

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
