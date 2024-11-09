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
    char c1, c2;

    while ((c1 = fgetc(file)) != EOF)
    {
        // Skip newlines
        if (c1 == '\n')
        {
            continue;
        }

        // Read second character of the pair
        c2 = fgetc(file);
        if (c2 == EOF || c2 == '\n')
        {
            // Handle last single character
            total_potions += get_base_potions(c1);
            break;
        }

        // Calculate potions for the pair
        int potions_c1 = get_base_potions(c1);
        int potions_c2 = get_base_potions(c2);

        // If both creatures are present (not 'x'), add one extra potion per creature
        if (c1 != 'x' && c2 != 'x')
        {
            potions_c1++;
            potions_c2++;
        }

        total_potions += potions_c1 + potions_c2;
    }

    fclose(file);
    printf("Part 2: %d\n", total_potions);
    return 0;
}
