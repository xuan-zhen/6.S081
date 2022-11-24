#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char c;
    int pid = fork();
    if (pid == 0) { // child
        read(p1[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], &c, 1);
        close(p2[1]);
        exit(0);
    } else {
        c = 'a';
        write(p1[1], &c, 1);
        close(p1[1]);
        read(p2[0], &c, 1);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}