#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int copy(int src_fd, int dest_fd)
{
    /*
        This function is responsible for the copying process
        in terms of the kernel API. It'll read bytes_read from 
        src_fd, then write them into dest_fd, until it reaches EOF.
    */

    char *buffer = (char *)malloc(80 * sizeof(char));
    int bytes_read, bytes_written;
    while ((bytes_read = read(src_fd, buffer, 80)) > 0)
    {
        if ((bytes_written = write(dest_fd, buffer, bytes_read)) < 0)
        {
            perror("Write:");
            return -1;
        }
    }
    if (bytes_read < 0)
    {
        perror("Read:");
        return -1;
    }
    return 0;
}

int checkAndCopy(char* src, char* dest)
{
    /*
        This function is responsible for opening files,
        and making sure every file has been opened correctly.
        Then it call copy() to perform the copying process.
    */

    int src_fd, dest_fd;
    if ((src_fd = open(src, O_RDONLY)) < 0)
    {
        perror("Open");
        return -1;
    }
    // 0644 for permissions, -rw--r--r--
    else if ((dest_fd = open(dest, O_WRONLY | O_CREAT, 0644)) < 0) {
        perror("Open:");
        return -1;
    }
    return copy(src_fd, dest_fd);
}

int main(int argc, char *argv[])
{
    char* src_file = argv[1];
    // if no arugments: print usage message
    if (argc < 3)
    {
        printf("usage: copyf <source> <dest> [<dest2> ...]\n");
        exit(0);
    }

    /* COPYING FIRST FILE */
    if (checkAndCopy(src_file, argv[2]) < 0)
    {
        exit(1);
    }
    else
    {
        printf("file copied successfully!\n");
    }

    /* IF MORE THAN ONE DEST_FILE PASSED */
    if (argc > 3)
    {
        for (int i = 2; i < argc; i++)
        {
            if (checkAndCopy(src_file, argv[i]) < 0)
            {
                printf("Error copying file %s\n", argv[i]);
            }
            else
            {
                printf("Copying file %s successful\n", argv[i]);
            }
        }
    }
    return 0;
}
