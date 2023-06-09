#include "../header/shell_helper.h"

void print(char *string) { interrupt(0x21, 0x0, string, 0, 0); }

void input(char *string) { interrupt(0x21, 0x1, string, 0, 0); }

// void describeCmd(char **args, int argc) {
//   int i;
//   char *str = "0. ";

//   for (i = 0; i < argc; i++) {
//     str[0]++;
//     print(str);
//     print(args[i]);
//     print("\n");
//   }
//   str[0] = '0';
// }

// void printHeaderShell() {
//   char *string = "Tugas Besar OS Thanos\n";
//   int i = 0;

//   print(string);
//   for (i = 0; i < strlen(string); i++) {
//     byte warna = 0xD;
//     putInMemory(0xB000, 0x8001 + 2 * i, warna);
//   }
// }

void print_fs_retcode(enum fs_retcode return_code) {
  switch (return_code) {
  case FS_SUCCESS:
    print("Success\n");
    break;
  case FS_R_NODE_NOT_FOUND:
    print("Not found\n");
    break;
  case FS_R_TYPE_IS_FOLDER:
    print("Type is folder\n");
    break;
  case FS_W_FILE_ALREADY_EXIST:
    print("Already exist\n");
    break;
  case FS_W_NOT_ENOUGH_STORAGE:
    print("Not enough\n");
    break;
  case FS_W_MAXIMUM_NODE_ENTRY:
    print("Max node entry\n");
    break;
  case FS_W_MAXIMUM_SECTOR_ENTRY:
    print("Max sector entry\n");
    break;
  case FS_W_INVALID_FOLDER:
    print("Invalid folder\n");
    break;
  }
}

// void testWrite(char* name, char* buffer) {
//   struct file_metadata metadata;
//   enum fs_retcode return_code;

//   metadata.buffer = buffer;
//   metadata.node_name = name;
//   metadata.parent_index = FS_NODE_P_IDX_ROOT;
//   metadata.filesize = 512;

//   print("----\n");
//   interrupt(0x21, 0x5, &metadata, &return_code, 0);
//   print("return code: ");
//   printHex(return_code);
//   print("\n");
// }

void clearStr(char* str) {
  int i;
  for (i = 0; str[i] != '\0'; i++) {
    str[i] = '\0';
  }
}

void cwd(byte cur_dir) {
  char buffer[1024];
  char temp[16];
  char* cur_name = "";
  byte child[64];
  byte u;

  if (cur_dir == FS_NODE_P_IDX_ROOT) {
    print("/");
  } else {
    interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);


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

  print(cur_name);
  clearStr(cur_name);
}

void printHex(byte b) {
  char str[3];
  byte temp;

  print("0x");
  temp = div(b, 16);
  str[0] = temp > 9 ? temp + 55 : temp + 48;

  temp = mod(b, 16);
  str[1] = temp > 9 ? temp + 55 : temp + 48;
  str[2] = '\0';
  print(str);
}

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

void deleteFile(byte cur_dir, char* filename) {
  char sector_buffer[1024];
  char node_buffer[512];
  char map_buffer[512];
  int i, j;
  byte idx_node;

  interrupt(0x21, 0x2, sector_buffer, FS_SECTOR_SECTOR_NUMBER);
  interrupt(0x21, 0x2, node_buffer, FS_NODE_SECTOR_NUMBER);
  interrupt(0x21, 0x2, node_buffer + 512, FS_NODE_SECTOR_NUMBER + 1);
  interrupt(0x21, 0x2, map_buffer, FS_MAP_SECTOR_NUMBER);

  // cari index dari filename di node_buffer
  for (i = 0; i < 64; i++) {
    if (node_buffer[i * 16] == cur_dir && strcmp(node_buffer + i * 16 + 2, filename)) {
      idx_node = i;
      break;
    }
  }

  // telusuri di sector_buffer dan hapus di map_buffer
  for (i=0; i<16; i++) {
    j = sector_buffer[idx_node * 16 + i];
    if (j != 0) {
      map_buffer[j] = 0;
      sector_buffer[idx_node * 16 + i] = 0x0;
    }
  }

  // hapus node_buffer
  node_buffer[idx_node * 16] = 0;
  node_buffer[idx_node * 16 + 1] = 0;
  for (i = 2; i < 16; i++) {
    node_buffer[idx_node * 16 + i] = '\0';
  }

  interrupt(0x21, 0x3, sector_buffer, FS_SECTOR_SECTOR_NUMBER);
  interrupt(0x21, 0x3, node_buffer, FS_NODE_SECTOR_NUMBER);
  interrupt(0x21, 0x3, node_buffer + 512, FS_NODE_SECTOR_NUMBER + 1);
  interrupt(0x21, 0x3, map_buffer, FS_MAP_SECTOR_NUMBER);
}

bool getParentPath(char *child, char *parent) {
  int idxLastSlash, i;

  strcpy(parent, child);

  idxLastSlash = -1;
  for (i = 0; parent[i] != '\0' ; i++) {
    if (parent[i] == '/') {
      idxLastSlash = i;
    }
  }

  if (idxLastSlash == -1) {
    // parent = "/";
    return false;
  } else if(idxLastSlash==0) {
    idxLastSlash = 1;
  } //root

  while (1) {
    if (parent[idxLastSlash] == '\0') {
      break;
    }
    parent[idxLastSlash] = '\0';
    idxLastSlash++;
  }

  return true;
}

