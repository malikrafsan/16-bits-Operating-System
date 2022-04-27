#include "../header/command.h"
#include "../header/textio.h"
#include "../header/program.h"
#include "../header/string.h"
#include "../header/shell_lib.h"

int main() {
  struct message msg;
  char buffer[1024];
  char **args;
  char parent_dest[64];
  char new_name[15];
  char* path_src;
  char* path_dest;
  int argc, i;
  byte idx_src, idx_dest, idx_parent_dest, cur_dir;
  bool success_src, success_dest, success_parent_dest, success_get_parent;

  getCurMessage(&msg);

  cur_dir = msg.current_directory;
  path_src = msg.arg1;
  path_dest = msg.arg2;
  
  idx_src = getIdxByPath(cur_dir, path_src, &success_src);
  if (!success_src) {
    puts("There is no ");
    puts(path_src);
    puts("\n");
    exit(msg.current_directory, msg.next_program_segment);
    return 0;
  }

  idx_dest = getIdxByPath(cur_dir, path_dest, &success_dest);

  if (success_dest) {
    puts(path_dest);
    puts(" exist\n");
    exit(msg.current_directory, msg.next_program_segment);
    return 0;
  }

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  success_get_parent = getParentPath(path_dest, parent_dest);

  if (!success_get_parent) {
    idx_parent_dest = FS_NODE_P_IDX_ROOT;
  } else {
    idx_parent_dest = getIdxByPath(cur_dir, parent_dest, &success_parent_dest);

    if (!success_parent_dest) {
      puts("Unknown error\n");
      exit(msg.current_directory, msg.next_program_segment);
      return 0;
    }
  }

  argc = splitStr(path_dest, args, '/');

  strcpy(new_name, args[argc - 1]);

  for (i=0;i < 14 && new_name[i] != '\0';i++) {
    buffer[idx_src * 16 + i + 2] = new_name[i];
  }
  for (;i < 14; i++) {
    buffer[idx_src * 16 + i + 2] = '\0';
  }

  buffer[idx_src * 16] = idx_parent_dest;

  interrupt(0x21, 0x3, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x3, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  exit(msg.current_directory, msg.next_program_segment);
  return 0;
}
