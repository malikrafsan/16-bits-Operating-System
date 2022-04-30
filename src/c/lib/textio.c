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

void puts_fs_retcode(enum fs_retcode return_code) {
  switch (return_code) {
  case FS_SUCCESS:
    puts("Success\n");
    break;
  case FS_R_NODE_NOT_FOUND:
    puts("Not found\n");
    break;
  case FS_R_TYPE_IS_FOLDER:
    puts("Type is folder\n");
    break;
  case FS_W_FILE_ALREADY_EXIST:
    puts("Already exist\n");
    break;
  case FS_W_NOT_ENOUGH_STORAGE:
    puts("Not enough\n");
    break;
  case FS_W_MAXIMUM_NODE_ENTRY:
    puts("Max node entry\n");
    break;
  case FS_W_MAXIMUM_SECTOR_ENTRY:
    puts("Max sector entry\n");
    break;
  case FS_W_INVALID_FOLDER:
    puts("Invalid folder\n");
    break;
  }
}

void putsHex(byte b) {
  char str[3];
  byte temp;

  puts("0x");
  temp = div(b, 16);
  str[0] = temp > 9 ? temp + 55 : temp + 48;

  temp = mod(b, 16);
  str[1] = temp > 9 ? temp + 55 : temp + 48;
  str[2] = '\0';
  puts(str);
}

void printHeader() {
  char *string = "Tugas Besar OS Thanos\n";
  // int i = 0;

  puts(string);
  // for (i = 0; i < strlen(string); i++) {
  //   byte warna = 0xCF;
  //   putInMemory(0xB000, 0x8001 + 2 * i, warna);
  // }
}