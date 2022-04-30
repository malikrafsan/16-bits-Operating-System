#include "../header/command.h"
#include "../header/textio.h"
#include "../header/program.h"
#include "../header/string.h"
#include "../header/fileio.h"

int main() {
  struct message msg;
  char buffer[1024];
  byte res;
  bool success;
  byte parent;
  char* path;

  getCurMessage(&msg);

  parent = msg.current_directory;
  path = msg.arg1;

  res = getIdxByPath(parent, path, &success);

  if (!success) {
    puts("No such directory\n");
    exit(msg.current_directory, msg.next_program_segment);
    return 0;
  }

  // interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  // interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);
  readSectorLib(buffer, FS_NODE_SECTOR_NUMBER);
  readSectorLib(buffer + 512, FS_NODE_SECTOR_NUMBER + 1);

  if (res==FS_NODE_P_IDX_ROOT || buffer[res * 16 + 1] == FS_NODE_S_IDX_FOLDER) {
    puts("CD SUCCESS\n");
    exit(res, msg.next_program_segment);
  } else {
    puts("Not a directory\n");
    exit(msg.current_directory, msg.next_program_segment);
  }

  return 0;
}