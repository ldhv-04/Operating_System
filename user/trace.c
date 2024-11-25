#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "usage: trace mask command\n");
        exit(1);
    }
    
    int mask = atoi(argv[1]);
    trace(mask); // bật theo dõi với mask đã cho

    // Chạy lệnh
    if (fork() == 0) {
        exec(argv[2], &argv[2]);
    } else {
        wait(0);
    }
    
    exit(0);
}