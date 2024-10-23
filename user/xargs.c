#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"  // MAXARG

#define IS_BLANK(chr) (chr == ' ' || chr == '\t')

int main(int argc, char *argv[]) {
    char buffer[2048], curChar;
    char *curPos = buffer;
    char *tmp[MAXARG];
    int j, maxArgs = 1;  // Doi so mac dinh la 1. Vi trong mang thuc thi tmp, tmp[0] luon la echo, j dai dien cho so luong doi so co trong tmp 
    int blankCount = 0;
    int bufferOffset = 0; // Chi so cua mang buffer

    //Kiem tra tuy chon -n 
    if (argc > 2 && strcmp(argv[1], "-n") == 0) {
        maxArgs = atoi(argv[2]);  // 
		argc -= 2;
        argv += 2;
    }

    // sao chep cac doi so con lai vao tmp[]
    for (j = 1; j < argc; j++) {
        tmp[j - 1] = argv[j];
    }
    --j;

    while (read(0, &curChar, 1) > 0) {   // Doc du lieu tu stdin
        if (IS_BLANK(curChar)) {
            blankCount++;
            continue;
        }
        if (blankCount) {
            buffer[bufferOffset++] = 0;
            tmp[j++] = curPos;
            curPos = buffer + bufferOffset;
            blankCount = 0;
        }
        if (curChar != '\n') {
            buffer[bufferOffset++] = curChar;
        } else {
            buffer[bufferOffset++] = 0;
            tmp[j++] = curPos;
            curPos = buffer + bufferOffset;

            // Kiem tra xem da thu thap duoc day du doi so chua
            if ((j - (argc - 1)) >= maxArgs) { 
                if (!fork()) {
                    tmp[j] = 0;  // Dam bao danh sach ket thuc bang null
                    exec(tmp[0], tmp);
                    exit(0);
                }
                wait(0);
                j = argc - 1;  // reset lai j sau khi thuc thi het doi so
            }
        }
    }
// 

    exit(0);
}
