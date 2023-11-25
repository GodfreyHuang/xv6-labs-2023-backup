#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void pipeline(int msg) 
{
    int p1[2];
    int readMsg, prime, num, status = 0;
    readMsg = read(msg, &prime, sizeof(int));
    if(!readMsg)
        return;

    printf("prime %d\n", prime);
    pipe(p1);
    int pid = fork();
    if(pid < 0)
    {
        printf("fork error!\n");
        exit(1);
    }
    else if(pid > 0) //parent
    {
        close(p1[0]);
        while((readMsg = read(msg, &num, sizeof(int))))
        {
            if(num % prime)
                write(p1[1], &num, sizeof(int));
        }
        close(p1[1]);
    }
    else //child
    {
        close(p1[1]);
        pipeline(p1[0]);
        close(p1[0]);
        exit(0);
    }
    wait(&status);
}

int main(int argc, char *argv[])
{
    int p[2];
    int status = 0;
    pipe(p);
    int pid = fork();
    if(pid < 0)
    {
        printf("fork error!\n");
        exit(1);
    }
    else if(pid > 0)
    {
        close(p[0]);
        for(int i = 2; i <= 35; ++i)
            write(p[1], &i, sizeof(int));
        close(p[1]);
    }
    else
    {
        close(p[1]);
        pipeline(p[0]);
        close(p[0]);
        exit(0);
    }
    wait(&status);
    exit(0);
}
