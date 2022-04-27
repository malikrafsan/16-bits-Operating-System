#include "../header/command.h"
#include "../header/textio.h"
#include "../header/program.h"
#include "../header/string.h"
#include "../header/shell_lib.h"

int main() {
  struct message msg;
  struct file_metadata metadata;
  enum fs_retcode return_code;
  byte curdir;
  char *name;

  getCurMessage(&msg);

  curdir = msg.current_directory;
  name = msg.arg1;

  if (strlen(name) > 14) {
    puts("Folder name too long");
    exit(curdir, msg.next_program_segment);
  }

  metadata.buffer = 0x00;
  metadata.node_name = name;
  metadata.parent_index = curdir;
  metadata.filesize = 0;

  interrupt(0x21, 0x5, &metadata, &return_code, 0);
  puts_fs_retcode(return_code);

  exit(curdir, msg.next_program_segment);
}