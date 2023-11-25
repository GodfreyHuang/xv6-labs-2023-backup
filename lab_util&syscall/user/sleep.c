#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        const char *errorText = "Error. Please enter time for sleep!\n";
        write(1, errorText, strlen(errorText));
        exit(1);
    } else if (argc > 2) {
        const char *errorText =
            "Error. Please enter in the form of: sleep <number>\n";
        write(1, errorText, strlen(errorText));
        exit(1);
    }

    sleep(atoi(argv[1]));

    exit(0);
}
