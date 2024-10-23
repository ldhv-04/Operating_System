#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int leftPipe[2]) __attribute__((noreturn));

void primes(int leftPipe[2]) {
    int prime=-1, num=-1;
    
    close(leftPipe[1]);

    if (read(leftPipe[0], &prime, sizeof(prime)) == 0) {
        close(leftPipe[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int rightPipe[2];
    pipe(rightPipe);

    if (fork() == 0) {
        close(leftPipe[0]);
        primes(rightPipe);
    } else {
        close(rightPipe[0]);

        while (read(leftPipe[0], &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(rightPipe[1], &num, sizeof(num));
            }
        }

        close(leftPipe[0]);
        close(rightPipe[1]); 
        wait(0);
        exit(0);
    }
}

int main() {
    int pipeMain[2];
    pipe(pipeMain);

    if (fork() == 0) {
        primes(pipeMain);
    } else {
        close(pipeMain[0]);
        
        for (int i = 2; i <= 280; i++) {
            write(pipeMain[1], &i, sizeof(i));
        }

        close(pipeMain[1]); 
        wait(0); 
        exit(0);
    }
}