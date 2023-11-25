#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(char *argv[])
{
    int pid = 0, status = 0;
    pid = fork(); //使用fork呼叫子行程來執行命令
    if(pid < 0)
    {
        printf("fork error!\n");
        exit(1);
    }
    else if(pid > 0)
    {
        wait(&status);
        return;
    }
    else
    {
        exec(argv[0], argv);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2 || argc > MAXARG - 1)
    {
        printf("Error: xargs <cmd> {args}, args must be in the range of 1 ~ 32\n");
        exit(1);
    }

    int i = 0;
    char buf[512], *order[MAXARG];
    //初始化buf的記憶體空間
    memset(buf, 0, sizeof(buf));
    //將argv[]記錄於order
    for(i = 1; i < MAXARG; ++i)
    {
        if(i < argc)
            order[i - 1] = argv[i];
        else
            order[i] = 0;
    }

    int ii = 0;
    char c;
    while(read(0, &c, 1) > 0)
    {
        if(c != '\n') //不等於換行表示還需要將讀取到的c放入buf內
        {
            buf[ii] = c;
            ++ii;
        }
        else
        {
            if(ii != 0) //如果ii不為0則代表有指令需要被執行
            {
                buf[ii] = '\0'; //下字串結尾符號
                order[argc - 1] = buf;
                xargs(order); //執行命令
                ii = 0;
            }
        }
    }
    
    if(ii != 0)
    {
        buf[ii] = '\0';
        order[argc - 1] = buf;
        xargs(order);
    }

    exit(0);
}
