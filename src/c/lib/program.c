#include "../header/program.h"
#include "../header/shell_lib.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void exec(struct file_metadata *metadata, int segment) {
    // puts("EXECUTE PROGRAM WITH SEGMENT: ");
    // putsIntFlipped(segment);
    // puts("\n");
    
    interrupt(0x21,0x6,metadata,segment,0);
}

void exit(byte cur_dir, int next_program_segment) {
    struct file_metadata metadata;
    struct message msg;
    bool success;

    getMessage(&msg, next_program_segment);
    msg.current_directory = cur_dir;
    setMessage(&msg,next_program_segment);
    // putsMsg(&msg);
    metadata.node_name = msg.name;
    if(msg.isLocalProgram) {
        metadata.parent_index = msg.current_directory;
    } else {
        metadata.parent_index = getIdxByPath(0xFF,"/bin",&success);
    }
    exec(&metadata, next_program_segment);
}