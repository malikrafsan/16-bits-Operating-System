#include "../header/textio.h"
#include "../header/std_lib.h"
#include "../header/utils.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

int puts(char *string) {
  interrupt(0x21, 0x0, string, 0, 0);
  return 0;
}

int gets(char *string) {
  interrupt(0x21, 0x1, string, 0, 0);
  return 0;
}

int putsIntFlipped(int x) {
  int i=0;
  int counter = 0;
  int copy = x;
  char buf[2];
  *(buf + 1) = 0x00;

  while (copy != 0) {
    copy = div(copy, 10);
    counter++;
  }

  for (i = 0; i < counter; i++) {
    *buf = mod(x,10) + 0x30;
    puts(buf);
    x = div(x, 10);
  }
  return 0;
}

int putsMsg(struct message *msg) {
  puts("MESSAGE:\n");
  puts("current_directory: ");
  putsIntFlipped(msg->current_directory);
  puts("\n");
  puts("isLocalProgram: ");
  putsIntFlipped(msg->isLocalProgram);
  puts("\n");
  puts("name: ");
  puts(msg->name);
  puts("\n");
  puts("arg1: ");
  puts(msg->arg1);
  puts("\n");
  puts("arg2: ");
  puts(msg->arg2);
  puts("\n");
  puts("arg3: ");
  puts(msg->arg3);
  puts("\n");
  puts("next_program_segment: ");
  putsIntFlipped(msg->next_program_segment);
  puts("\n");
  puts("other: ");
  puts(msg->other);
  puts("\n");
  return 0;
}
