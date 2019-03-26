#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// 2- Check if you can write characters starting from position 15.
int main()
{
    char *file_name = "./text2";
    // storing file descriptor
    int open_file = open(file_name, O_WRONLY);

    // buffer for writing to open_file
    char *write_buffer_open_file = "ABCDEFG";

    if (lseek(open_file, 14, SEEK_SET) > 0)
    {
        if (write(open_file, write_buffer_open_file, 7) >= 0)
        {
            printf("%s\n", "file has been written it.");
            return 0;
        }
        perror("Write");
        exit(1);
    }
    else
    {
        perror("lseek");
        exit(1);
    }
    return 0;
}
