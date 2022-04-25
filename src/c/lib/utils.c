#include "../header/utils.h"
#include "../header/textio.h"

void getMessage(struct message *msg, int program_segment) {
    int sector_number = (program_segment-0x1000)/0x1000 + 0x103; // nanti pake div
    readSectorLib(msg,sector_number);
}

void getCurMessage(struct message *msg) {
    int cur_segment = getCurrentSegment();
    puts("cur segment: ");
    putsIntFlipped(cur_segment);
    puts("\n");
    getMessage(msg,cur_segment);
}

void set_MultiMsg(char* input_buf, byte curdir) {
    int ctProgram;
    char** cmd;
    int argc;
    char** args[5];
    int i;
    struct message msg;
    char c[2];

    // split dari _;_
    ctProgram = splitStr(input_buf, cmd, ';');
    msg.current_directory = curdir;
    msg.isLocalProgram = false;
    strcpy(msg.name,"shell");
    msg.next_program_segment = 0x3000;
    setMessage(&msg, 0x2000);

    // split dengan " "
    for(i=0;i<ctProgram;i++) {
        argc = splitStr(cmd[i], args[i], ' ');
        bounded_strcpy(c,args[i][0],2);
        if(strcmp(c,"./")) {
            msg.isLocalProgram = true;
            strcpy(msg.name, args[i][0]+2);
        } else {
            msg.isLocalProgram = false;
            strcpy(msg.name, args[i][0]);
        }
        // puts("msg.name: ");
        // puts(args[i][0]);
        // puts("\n");

        if(argc>=2) {
            strcpy(msg.arg1,args[i][1]);
        } else {
            msg.arg1[0] = 0x0;
        }
        if(argc>=3) {
            strcpy(msg.arg2,args[i][2]);
        } else {
            msg.arg2[0] = 0x0;
        }
        if(argc>=4) {
            strcpy(msg.arg3,args[i][3]);
        } else {
            msg.arg3[0] = 0x0;
        }

        if(i==ctProgram-1) {
            msg.next_program_segment = 0x2000;
        } else {
            msg.next_program_segment = 0x4000+i*0x1000;
        }
        setMessage(&msg, 0x3000+i*0x1000);
        putsMsg(&msg);
    }
}

void setMessage(struct message *msg, int program_segment) {
    int sector_number = (program_segment-0x1000)/0x1000 + 0x103; // nanti pake div
    writeSectorLib(msg,sector_number);
}