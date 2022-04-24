#include "header/filesystem.h"

void exit() {
  struct file_metadata meta;
  meta.node_name    = "shell";
  meta.parent_index = 0x00;

  exec(meta, 0x2000);
}
