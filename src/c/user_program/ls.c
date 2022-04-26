#include "../header/command.h"
#include "../header/textio.h"
#include "../header/program.h"
#include "../header/string.h"

int main() {
  struct message msg;
  byte curdir;
  char buffer[1024];
  char name[15];
  int i, j, idxName;
  bool isEmpty;

  getCurMessage(&msg);

  curdir = msg.current_directory;
  puts("\nUSING LS PROGRAM\n");
  puts("CURDIR: ");
  putsIntFlipped(curdir);
  puts("\n");

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  isEmpty = true;
  for (i = 0; i < 64; i++) {
    if (buffer[i * 16] == curdir && strlen(buffer + i * 16 + 2) != 0) {
      if (buffer[i * 16 + 1] == FS_NODE_S_IDX_FOLDER) {
        puts("dir: ");
      } else {
        puts("file: ");
      }

      j = 0;
      idxName = i * 16 + 2;
      bounded_strcpy(name, buffer + idxName, 14);
      puts(name);
      puts("\n");
      // count++;
      isEmpty = false;
      clearStr(name);
    }
  }
  if (isEmpty) {
    puts("Empty directory\n");
  } else {
    puts("NOT EMPTY\n");
  }
  clearStr(buffer);

  exit(curdir, msg.next_program_segment);
}