/* Example - a simple C version of UNIX ls utility */

/*
Homework:
use another choose, and select functions, and sort(?) function.
*/

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE !FALSE

extern int alphasort();
extern int sizeSort(const struct dirent **e1, const struct dirent **e2);
char pathname[MAXPATHLEN];
int file_s = 64;

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        file_s = atoi(argv[1]);
    }
    else
    {
        printf("You didn't specify selecting file size\n");
        printf("*** Selection criteria size will be 64 bytes ***\n");
    }

    int count, i;
    struct dirent **files;
    int file_select(), special_select();

    if (getwd(pathname) == NULL)
    {
        printf("Error getting path\n");
        exit(0);
    }

    printf("Current Working Directory = %s\n", pathname);
    /* If no files found, make a non-selectable menu item */
    count = scandir(pathname, &files, special_select, sizeSort);
    if (count <= 0)
    {
        printf("No files in this directory\n");
        exit(0);
    }
    printf("Number of files = %d\n", count);
    for (i = 1; i < count + 1; ++i)
    {
        printf("%s ", files[i - 1]->d_name);
    }
    printf("\n"); /* flush buffer */
}

int file_select(struct direct *entry)
{
    char *ptr;
    char *rindex(char *s, char c);
    if ((strcmp(entry->d_name, ".") == 0) ||
        (strcmp(entry->d_name, "..") == 0))
        return (FALSE);
    /* Check for filename extensions */
    ptr = rindex(entry->d_name, '.');
    if ((ptr != NULL) && ((strcmp(ptr, ".c") == 0) || (strcmp(ptr, ".h") == 0) || (strcmp(ptr, ".o") == 0)))
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/*
    special function: select regular files of size >= file_s
*/

int special_select(struct direct *entry)
{
    struct stat buf;
    if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
    {
        return (FALSE);
    }

    /* access size of file using stat struct */
    if (stat(entry->d_name, &buf) < 0)
    {
        printf("error with file %s", entry->d_name);
    }
    else if (entry->d_type == DT_REG && buf.st_size >= file_s)
    {
        return (TRUE);
    }
    return (FALSE);
}

int sizeSort(const struct dirent **e1, const struct dirent **e2)
{
    struct stat buf_e1, buf_e2;
    int size_e1, size_e2;

    /* if error in reading stat of first file */
    if (stat((*e1)->d_name, &buf_e1) < 0)
    {
        printf("error with file %s", (*e1)->d_name);
    }
    /* if error in reading stat of first file */
    else if (stat((*e2)->d_name, &buf_e2) < 0)
    {
        printf("error with file %s", (*e2)->d_name);
    }
    /* if everything is good */
    else
    {
        if (buf_e1.st_size < buf_e2.st_size)
        {
            return 1;
        }
        else if (buf_e1.st_size > buf_e2.st_size)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
}