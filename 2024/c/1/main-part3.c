#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int get_base_potions(char creature)
{
    switch (creature)
    {
    case 'A':
        return 0;
    case 'B':
        return 1;
    case 'C':
        return 3;
    case 'D':
        return 5;
    case 'x':
        return 0;
    default:
        return 0;
    }
}

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

    int total_potions = 0;
    char c1, c2, c3;

    while ((c1 = fgetc(file)) != EOF)
    {
        // Skip newlines
        if (c1 == '\n')
            continue;

        // Read the next two characters of the triplet
        c2 = fgetc(file);
        if (c2 == '\n')
            c2 = 'x'; // Treat newline as empty
        if (c2 == EOF)
            c2 = 'x'; // If at end, treat as empty

        c3 = fgetc(file);
        if (c3 == '\n')
            c3 = 'x';
        if (c3 == EOF)
            c3 = 'x';

        // Calculate base potions
        int potions_c1 = get_base_potions(c1);
        int potions_c2 = get_base_potions(c2);
        int potions_c3 = get_base_potions(c3);

        // Add extra potions based on the number of creatures in the group
        if (c1 != 'x' && c2 != 'x' && c3 != 'x')
        {
            // All three creatures present
            potions_c1 += 2;
            potions_c2 += 2;
            potions_c3 += 2;
        }
        else if ((c1 != 'x' && c2 != 'x') || (c1 != 'x' && c3 != 'x') || (c2 != 'x' && c3 != 'x'))
        {
            // Two creatures present
            if (c1 != 'x')
                potions_c1 += 1;
            if (c2 != 'x')
                potions_c2 += 1;
            if (c3 != 'x')
                potions_c3 += 1;
        }

        // Sum up potions for the triplet
        total_potions += potions_c1 + potions_c2 + potions_c3;
    }

    fclose(file);
    printf("Part 3: %d\n", total_potions);
    return 0;
}
