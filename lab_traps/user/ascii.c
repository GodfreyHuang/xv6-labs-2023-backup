#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    unsigned int i = 0x00646c72;
    printf("H%x Wo%s\n", 57616, &i);

    exit(0);
}