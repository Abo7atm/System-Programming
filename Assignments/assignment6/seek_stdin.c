#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// 3- What happens if I use lseek on StdIn
int main()
{
    int stdin_fd = 0;

    // buffer for writing to open_file
    char *read_from_user = (char *)malloc(15 * sizeof(char));

    int stdin_seek = lseek(stdin_fd, 15, SEEK_CUR);
    if (stdin_seek > 0)
    {
        printf("%d\n", stdin_seek);
        printf("%s", "Please enter a string(word): ");
        scanf("%s", read_from_user);
    }
    else
    {
        perror("lseek");
        exit(1);
    }
    printf("%s\n", read_from_user);
    return 0;
}