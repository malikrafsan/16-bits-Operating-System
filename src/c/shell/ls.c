#include "../header/command.h"

int main() {
  // params
  byte curDir;

  char buffer[1024];
  char name[15];
  int i, j, idxName;
  bool isEmpty;

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  // count = 0;
  isEmpty = true;
  for (i = 0; i < 64; i++) {
    if (buffer[i * 16] == curDir && strlen(buffer + i * 16 + 2) != 0) {
      if (buffer[i * 16 + 1] == FS_NODE_S_IDX_FOLDER) {
        print("dir: ");
      } else {
        print("file: ");
      }

      j = 0;
      idxName = i * 16 + 2;
      bounded_strcpy(name, buffer + idxName, 14);
      print(name);
      print("\n");
      // count++;
      isEmpty = false;
      clearStr(name);
    }
  }
  if (isEmpty) {
    print("Empty directory\n");
  }
  clearStr(buffer);
}