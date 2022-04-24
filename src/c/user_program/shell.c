#include "../header/std_type.h"
#include "../header/textio.h"
#include "../header/string.h"

int main() {
  char input_buf[64];
  char** args;
  int argc;

  puts("Halo dari shell user program!\r\n");
  while (1) {
    puts("OSThanos:/$? "); 
    gets(input_buf);

    argc = splitStr(input_buf, args, ' ');

    if (strcmp(args[0], "cd")) {
      puts("USE COMMAND CD\n");
    } else if (strcmp(args[0], "ls")) {
      puts("USE COMMAND LS\n");
    } else if (strcmp(args[0], "mkdir")) {
      puts("USE COMMAND MKDIR\n");
    } else if (strcmp(args[0], "exit")) {
      puts("USE COMMAND EXIT\n");
    } else {
      puts(input_buf);
      puts("\nUNKNOWN COMMAND\n");
    }
  }
}
