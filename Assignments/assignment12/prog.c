#include <stdlib.h>
#include <stdio.h>

void f1()
{
    printf("I would like A+ please.\n");
}

void f2()
{
    printf("World\n");
}

void f3()
{
    printf("Hello, ");
}

int main(int argc, char* argv[])
{

    if (argc < 2 || argc > 4)
    {
        printf("Usage: atexit <option>\n");
        printf("Options:\n");
        printf("1: case 1, the main prints a message and then returns 0.\n");
        printf("2: case 2, the main does a calculation with a division by zero.\n");
        exit(0);
    }

    atexit(f1);
    atexit(f2);
    atexit(f3);

    switch (atoi(argv[1]))
    {
        case 1:
            printf("<<< This is normal case >>>\n");
            break;
        case 2:
            printf("<<< Division by zero is DEFINED! >>>\n");
            int j = 1 / 0;
            printf("value: %d\n", 1+j);
            break;
        default:
            break;
    }

    exit(0);
}