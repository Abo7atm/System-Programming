#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// 4- What happens if I use lseek on StdOut
int main()
{
    int stdout_fd = 1;
    char *read_from_user = (char *)malloc(15 * sizeof(char));

    int stdout_seek = lseek(stdout_seek, 10, SEEK_CUR);
    if (stdout_seek > 0)
    {
        printf("%s\n", "this_is_a_long_string");
    }
    else
    {
        perror("lseek");
        exit(1);
    }
    printf("%s\n", read_from_user);
    return 0;
}