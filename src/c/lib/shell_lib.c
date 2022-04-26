#include "../header/shell_lib.h"
#include "../header/filesystem.h"

byte getIdxByPath(byte cur_dir, char* path, bool *success) {
  char buffer[1024];
  int i, n, i1;
  char name[15];
  char c;
  bool flag;

  i1 = 0;
  if (path[0] == '/') {
    cur_dir = FS_NODE_P_IDX_ROOT;
    i1 = 1;
  }

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  // for (it = 0; it < argc; it++) {
  //   printHex(it);
  //   print(". ");
  //   print(args[it]);
  //   print("\n");
  // }
  // print("\n");
  *success = true;
  while(path[i1]!='\0') {
    n = 0;
    while(path[i1+n]!='\0' && path[i1+n]!='/') {
      n++;
    }
    c = path[i1+n];
    path[i1+n] = '\0';
    // memcpy(name, &path[i], n);
    strcpy(name,&(path[i1]));
    // name[n] = '\0';
    path[i1+n] = c;

    flag = false;
    // print(args[it]);
    // print(" -> ");
    if (strcmp(name, "..")) {
      if(cur_dir != FS_NODE_P_IDX_ROOT) {
        cur_dir = buffer[cur_dir * 16];
        flag = true;
      } else {
        flag = false;
      }
    } else {
      for (i = 0; i < 64; i++) {
        if (buffer[i * 16] == cur_dir &&
            strcmp(buffer + i * 16 + 2, name)) {
          // print("FOUND");
          cur_dir = i;
          flag = true;
          break;
        }
      }
    }
    if(!flag) {
      *success = false;
      return cur_dir;
    } 

    clearStr(name);
    i1 +=n;
    while(path[i1]!='\0' && path[i1]=='/') {
      i1++;
    }
  }

  clearStr(buffer);

  return cur_dir;
}