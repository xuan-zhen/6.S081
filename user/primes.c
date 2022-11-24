#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv []) {
    int p1[2];
    int p2[2];
    int num, prime, pid, piped, forked;
    pipe(p1);
    pid = fork();
    if (pid == 0) {
        do
        {
            piped = 0;
            forked = 0;
            close(p1[1]);
            p2[0] = dup(p1[0]);
            close(p1[0]);
            if(read(p2[0], &prime, 4) > 0) {
                printf("prime %d\n", prime);
            }
            while (read(p2[0], &num, 4) > 0) {
                if (num % prime == 0)
                    continue;
                else {
                    if (piped == 0) {
                        pipe(p1);
                        piped = 1;
                    }
                    if (forked == 0) {
                        forked = 1;
                        pid = fork();
                    }
                    if (pid > 0) {
                        write(p1[1], &num, 4);                        
                    } else {
                        break;
                    } 
                }
            }
            if (pid > 0 || (forked == 0 && pid ==0)) {
                close(p2[0]);
                close(p1[1]);
                wait(0);
                exit(0);
            }
        } while (1);
    } else {
        close(p1[0]);
        for (int i = 2; i <= 35; i++) {
            num = i;
            write(p1[1], &num, 4);
        }
        close(p1[1]);
        wait(0);
        exit(0);
    }
}