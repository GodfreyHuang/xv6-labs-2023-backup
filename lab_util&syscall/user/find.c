#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

// same code in /user/ls.c
char *fmtname(char *path)
{
    // static char buf[DIRSIZ+1]; //Original code from ls.c
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
    // Return blank-padded name.
    /*  //Original code from ls.c
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
    */
}

void find(char *path, char *target)
{
    // same code in /user/ls.c
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_DEVICE:
    case T_FILE:
        //---add---
        char *pp = fmtname(path);
        if (!strcmp(pp, target))
            printf("%s\n", path);
        //---------
        // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
        // //Original code from ls.c
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))  // add
                continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            /* //Original code from ls.c
            if(stat(buf, &st) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
            */
            find(buf, target);  // add
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: find <path> <name>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
