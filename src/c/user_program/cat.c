#include "../header/command.h"
#include "../header/textio.h"
#include "../header/program.h"
#include "../header/string.h"
#include "../header/fileio.h"

int main() {
  struct message msg;
  struct file_metadata metadata;
  enum fs_retcode return_code;
  byte buftemp[16 * 512];

  getCurMessage(&msg);

  metadata.buffer = buftemp;
  metadata.node_name = msg.arg1;
  metadata.parent_index = msg.current_directory;

  // interrupt(0x21, 0x4, &metadata, &return_code, 0);
  readLib(&metadata, &return_code);
  
  if(return_code==FS_SUCCESS) {
    puts(metadata.buffer);
    puts("\n");
  }
  puts_fs_retcode(return_code);

  exit(msg.current_directory, msg.next_program_segment);
  return 0;
}