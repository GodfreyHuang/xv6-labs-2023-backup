#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int cpid, status = 0, p1[2], p2[2]; // p1: parent -> child, p2: child -> parent
    char *buf = malloc(5); //4 char + end sign
    pipe(p1);
    pipe(p2);

    cpid = fork();
    if(cpid < 0)
    {
        printf("fork error!\n");
        exit(1);
    }

    if(cpid > 0) //parent
    {
        close(p1[0]);
        write(p1[1], "ping", 4);
        close(p1[1]);

        wait(&status);
        read(p2[0], buf, 4);
        close(p2[0]);
        close(p2[1]);

        printf("%d: received %s\n", getpid(), buf);
        exit(0);
    }
    else //child
    {
        close(0);
        read(p1[0], buf, 4);
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        write(p2[1], "pong", 4);
        close(p2[1]);

        printf("%d: received %s\n", getpid(), buf);
        exit(0);
    }
    free(read);
    /*
    int p[2], pid;
    pipe(p);
    pid = fork();
    if(pid > 0) {
        write(p[1], "Parent\n", 8); //
        wait(0);
        close(p[0]); //
        close(p[1]); //
        printf("%d: received pong\n", getpid());
    }
    else {
        close(0); //
        write(p[0], "Child\n", 7); //
        close(p[0]); //
        close(p[1]); //
        printf("%d: received ping\n", getpid());
        exit(0);
    }
    */
    exit(0);
}
