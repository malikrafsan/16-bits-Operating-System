#include "../header/command.h"

void ls(byte curDir) {
  char buffer[1024];
  char name[14];
  int i, j, idxName, count;

  interrupt(0x21, 0x2, buffer, FS_NODE_SECTOR_NUMBER, 0);
  interrupt(0x21, 0x2, buffer + 512, FS_NODE_SECTOR_NUMBER + 1, 0);

  count = 0;
  for (i = 0; i< 64; i++) {
    if (buffer[i * 16] == curDir) {
      if (buffer[i * 16 + 1] == FS_NODE_S_IDX_FOLDER) {
        print("dir: ");
      } else {
        print("file: ");
      }

      j = 0;
      idxName = i * 16 + 2;
      while (j < 14 && buffer[idxName + j] != '\0') {
        name[j] = buffer[idxName + j];
        j++;
      }
      print(name);
      print("\r\n\0");
      name[0] = '\0';
      count++;
    }
  }
  if (count == 0) {
    print("Empty directory\n");
  }
}
