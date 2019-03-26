#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int open_file = 0;

    // buffer for writing to open_file
    char *write_buffer_open_file = "ABCDEFG";
    char *read_from_scanf = (char *) malloc(sizeof(char)*7);

    if (write(open_file, write_buffer_open_file, 7) >= 0)
    {
        printf("%s\n", "");
        return 0;
    }
    perror("Write");
    exit(1);
}
