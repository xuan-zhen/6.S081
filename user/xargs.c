#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char buf[1024];
char* argvs[MAXARG];
int num;

int xexec(char *argv []) {
    if (fork() == 0) {
        // for (int argc = 0; argv[argc]; argc++)
        //     printf("%s\n", argv[argc]);
        exec(argv[0], argv);
        exit(0);
    }
    wait(0);
    return 0;
}

int xargs(int fd, int argc, char *argv []){
    int n, m;
    char *p, *q, *s;

    m = 0;
    num = 0;
    while((n = read(fd, buf+m, sizeof(buf)-m-1)) > 0) {
        m += n;
        buf[m] = '\0';
        p = buf;
        while ((q = strchr(p, '\n')) != 0) {
            num = 0;
            for (int i = 1; i < argc; i++) {
                argvs[num] = (char *)malloc(1024);
                memmove(argvs[num], argv[i], strlen(argv[i]));
                num++;
            }
            *q = 0;
            while ((s = strchr(p, ' ')) != 0) {
                argvs[num] = (char *)malloc(1024);
                memmove(argvs[num], p, s - p);
                num ++;
                p = s + 1;
            }
            argvs[num] = (char *)malloc(1024);
            memmove(argvs[num], p, q - p);
            num ++;
            xexec(argvs);
            p = q + 1;
        }
        if (m > 0) {
            m -= p - buf;
            memmove(buf, p, m);
        }
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    xargs(0, argc, argv);
    exit(0);
}