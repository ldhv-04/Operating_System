#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/syscall.h"






void pgaccess_test()
{
    char *ptr;
    uint64 bitmask;
    

    // Cap phat cho con tro ptr
    printf("pgaccess test\n");
    ptr = malloc(5 * PGSIZE);
    if(ptr == 0){
        printf("malloc failed\n");
        return;
    }
    
    // Truy cap vao trang thu 1 va trang thu 3
    ptr[0] = 1;
    ptr[2 * PGSIZE] = 1;

    if (pgaccess(ptr, 5, &bitmask) < 0) {
        printf("pgaccess failed\n");
        free(ptr);
        return;
    }

    printf("Bitmask: 0x%lx\n", bitmask);
    if ((bitmask & 0b101) != 0b101) {
        printf("pgaccess test failed, expected 0x5, got 0x%lx\n", bitmask);
    } else {
        printf("pgaccess test OK\n");
    }
    free(ptr);
}

int main(int argc, char *argv[]) {
    pgaccess_test();
    exit(0);
}