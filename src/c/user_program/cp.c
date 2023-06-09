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
  int i;
  byte current_dir;
  char* src;
  char* dst;

  getCurMessage(&msg);

  current_dir = msg.current_directory;
  src = msg.arg1;
  dst = msg.arg2;

  metadata.buffer = buftemp;
  metadata.node_name = src;
  metadata.parent_index = current_dir;

  // Copy buffer from src to metadata.buffer
  // interrupt(0x21, 0x4, &metadata, &return_code, 0);
  readLib(&metadata, &return_code);

  // jika src berupa folder, gagalkan!
  if (return_code == FS_R_TYPE_IS_FOLDER || return_code == FS_R_NODE_NOT_FOUND) {
    puts_fs_retcode(return_code);
    exit(msg.current_directory, msg.next_program_segment);
    return 0;
  }

  metadata.node_name = dst;

  // jika tidak ada, maka create file
  // interrupt(0x21, 0x5, &metadata, &return_code, 0);
  writeLib(&metadata, &return_code);

  // jika file sudah ada, maka hapus file yang ada, lalu create file baru
  // puts_fs_retcode(return_code);
  // puts("\n");
  if (return_code == FS_W_FILE_ALREADY_EXIST) {
    deleteFile(current_dir, dst);
    // puts("Berhasil delete: ");
    // puts(dst);
    // puts("\n");
    // interrupt(0x21, 0x5, &metadata, &return_code, 0);
    writeLib(&metadata, &return_code);
    // puts("Berhasil create");
    // puts(metadata.node_name);
    // puts("\n");
  }
  // puts("Sampai sini\n");
  puts_fs_retcode(return_code);
  
  exit(msg.current_directory, msg.next_program_segment);
  return 0;
}
