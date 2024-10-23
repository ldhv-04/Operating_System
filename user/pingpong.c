#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int p[2]; // mang 2 phan tu luu 2 dau cua pipe
    char buffer[10]; // buffer de luu pipe

    pipe(p); //pipe p[0] la dau doc, pipe[1] la dau ghi
    int pid;
    pid = fork(); // tao tien trinh 

    if(pid < 0){ // loi khi tao tien trinh con
        printf("Error\n");
        exit(0);
    }
    else if(pid == 0){ // tao tien trinh con thanh cong
        read(p[0], buffer, sizeof(buffer)); // tien trinh con doc du lieu tu p[0] va luu vao buffer voi kich thuoc la kich thuoc (buffer)
        printf("%d: received %s\n", getpid(), buffer); // in ra PID cung voi du lieu vua ghi
        write(p[1], "pong", 5); // tien trinh con gui thong diep "pong" qua dau ghi p[1] cho tien trinh cha
        close(p[0]); // dong 2 dau tien trinh
        close(p[1]);
        exit(0); // ket thuc tien trinh con
    }
    else{ //tao tien trinh cha
        write(p[1], "ping", 5); // tien trinh cha gui thong diep "ping" qua dau ghi [1] cho tien trinh con
        wait(0); // Doi tien trinh con hoan thanh 
        read(p[0], buffer, sizeof(buffer)); // doc thong diep cua tien trinh con qua dau doc p[0] va luu vao buffer
        printf("%d: received %s\n", getpid(), buffer); // in ra PID cung thong diep vua nhan tu tien trinh con
        close(p[0]);
        close(p[1]); // dong 2 dau tien trinh
    }

    exit(0); // ket thuc chuong trinh
}