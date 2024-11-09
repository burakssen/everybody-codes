#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    int count = 0;

    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == 'B')
            count++;
        else if (c == 'C')
            count += 3;
    }

    fclose(file);

    printf("Part 1: %d\n", count);
    return 0;
}
