#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// 1- Check if you can read characters from position 10 to 30
int main()
{
    int open_file;
    char *file_name = "./text";
    int num_bytes_to_read = 20;

    // checking and storing file descriptor
    if ((open_file = open(file_name, O_APPEND | O_RDONLY)) == -1)
    {
        perror("Open");
        exit(1);
    }

    // buffer for reading from open_file
    char *read_buffer_open_file = (char *)malloc(21 * sizeof(char));

    // findings: off_t doesn't allow for negative values.
    if (lseek(open_file, 9, SEEK_SET) > 0)
    {
        if (read(open_file, read_buffer_open_file, num_bytes_to_read) > 0)
        {
            printf("%s\n", read_buffer_open_file);
            return 0;
        }
        printf("%s\n", "there has been an error reading the file!");
        perror("read");
        return 0;
    }
    printf("%s\n", "there has been an error seeking the file!");
    perror("lseek");
    return 0;
}