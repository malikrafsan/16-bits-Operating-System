#include "../header/std_type.h"
#include "../header/textio.h"
#include "../header/string.h"
#include "../header/utils.h"
#include "../header/program.h"
#include "../header/sectorio.h"

void cwd(byte cur_dir);

void clearStr(char* str);

int main() {
  char input_buf[64];
  char** args;
  int argc;
  struct message msg;
  byte curdir;

  // puts("sizeof struct message: ");
  // putsIntFlipped(sizeof(struct message));
  // puts("\n");
  // puts("Halo dari shell user program!\r\n");
  while (1) {
    getCurMessage(&msg);
    // putsMsg(&msg);
    curdir = msg.current_directory;

    puts("curdir: ");
    putsIntFlipped(curdir);
    puts("\n");
    puts("OSThanos:"); 
    cwd(curdir);
    puts("$? ");
    gets(input_buf);

    set_MultiMsg(input_buf, curdir);
    getCurMessage(&msg);
    exit(curdir, msg.next_program_segment);
  }
}

void cwd(byte cur_dir) {
  char buffer[1024];
  char temp[16];
  char* cur_name = "";
  byte child[64];
  byte u;

  if (cur_dir == FS_NODE_P_IDX_ROOT) {
    puts("/");
  } else {
    readSectorLib(buffer,FS_NODE_SECTOR_NUMBER);
    readSectorLib(buffer+512, FS_NODE_SECTOR_NUMBER+1);

    // inisialisasi child
    u = cur_dir;
    while(buffer[16*u] != FS_NODE_P_IDX_ROOT) {
      child[buffer[16*u]] = u;
      u = buffer[16*u];
    }
    while (1) {
      bounded_strcpy(temp + 1, &buffer[16*u + 2], 14);
      temp[0] = '/';

      strcat(cur_name, temp);
      clearStr(temp);
      if (u == cur_dir) {
        break;
      }
      u = child[u];
    }
  }

  puts(cur_name);
  clearStr(cur_name);
}

void clearStr(char* str) {
  int i;
  for (i = 0; str[i] != '\0'; i++) {
    str[i] = '\0';
  }
}