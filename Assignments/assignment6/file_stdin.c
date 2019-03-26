#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// 5- Close your StdIn and open a regular file. What happens after lseek.
int main()
{
    int open_file;
    char *file_name = "./text";
    if (close(0) == -1)
    {
        perror("Close");
        exit(0);
    }
    // checking and storing file descriptor
    if ((open_file = open(file_name, O_RDONLY)) == -1)
    {
        perror("Open");
        exit(1);
    }
    // fd[0] == file_name;
    char *read_from_scan = (char *)malloc(10*sizeof(char));
    // findings: off_t doesn't allow for negative values.
    int seek_file = lseek(open_file, 10, SEEK_CUR);
    if (seek_file > 0)
    {
        scanf("%s", read_from_scan);
    }
    else
    {
        perror("lseek");
        exit(1);
    }
    printf("%s\n", read_from_scan);
    return 0;

}