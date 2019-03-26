#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct stat file_info;

    if (argc < 3 || argc > 5)
    {
        printf("Usage: hln <file1> <file2> <file3>\n");
        exit(0);
    }
    if (link(argv[1], argv[2]) < 0)
    {
        perror("Link");
        exit(1);
    }
    else if (link(argv[1], argv[3]) < 0)
    {
        perror("Link");
        exit(1);
    }

    for (int i = 1; i < 4; i++)
    {
        if (lstat(argv[i], &file_info) < 0)
        {
            perror("lstat");
            exit(1);
        }
        else {
            printf("Number of links to %s: %d\n", argv[i], (int)file_info.st_nlink);
        }
    }

    return 0;
}