#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_WORDS 1000
#define MAX_WORD_LEN 50
#define MAX_GRID_SIZE 1000

void to_upper(char *str)
{
    while (*str)
    {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

void reverse_string(char *str)
{
    for (int i = 0, j = strlen(str) - 1; i < j; i++, j--)
    {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int load_words(FILE *file, char words[MAX_WORDS * 2][MAX_WORD_LEN])
{
    char line[MAX_LINE];
    if (!fgets(line, sizeof(line), file))
        return 0;

    char *word_list = strchr(line, ':');
    if (!word_list)
        return 0;

    int word_count = 0;
    for (char *word = strtok(word_list + 1, ",\n"); word && word_count < MAX_WORDS; word = strtok(NULL, ",\n"))
    {
        while (*word == ' ')
            word++; // Trim leading spaces
        strncpy(words[word_count], word, MAX_WORD_LEN - 1);
        to_upper(words[word_count++]);
    }

    for (int i = 0; i < word_count; i++)
    { // Add reversed words
        strcpy(words[word_count + i], words[i]);
        reverse_string(words[word_count + i]);
    }
    return word_count * 2;
}

int read_grid(FILE *file, char grid[MAX_GRID_SIZE][MAX_GRID_SIZE], int *width)
{
    char line[MAX_LINE];
    int height = 0;
    *width = 0;
    fgets(line, sizeof(line), file); // Skip empty line

    while (fgets(line, sizeof(line), file) && height < MAX_GRID_SIZE)
    {
        to_upper(line);
        line[strcspn(line, "\n")] = 0; // Remove newline
        int line_len = strlen(line);

        if (*width == 0)
            *width = line_len;
        if (line_len != *width || line_len >= MAX_GRID_SIZE)
        {
            fprintf(stderr, "Error: Invalid grid dimensions\n");
            exit(1);
        }

        strcpy(grid[height++], line);
    }
    return height;
}

char get_wrapped_char(const char grid[MAX_GRID_SIZE][MAX_GRID_SIZE], int row, int col, int height, int width)
{
    return (row < 0 || row >= height) ? '\0' : grid[row][((col % width) + width) % width];
}

void mark_word_in_grid(int used[MAX_GRID_SIZE][MAX_GRID_SIZE], int row, int col, int length, int height, int width, int row_step, int col_step)
{
    for (int i = 0; i < length; i++)
    {
        int r = row + i * row_step;
        int c = ((col + i * col_step) % width + width) % width;
        if (r < 0 || r >= height)
            break;
        used[r][c] = 1;
    }
}

int count_runic_symbols(const char grid[MAX_GRID_SIZE][MAX_GRID_SIZE], const char words[MAX_WORDS * 2][MAX_WORD_LEN], int word_count, int height, int width)
{
    int used[MAX_GRID_SIZE][MAX_GRID_SIZE] = {0};

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            for (int w = 0; w < word_count; w++)
            {
                int length = strlen(words[w]);

                int match = 1;
                for (int i = 0; i < length && match; i++)
                {
                    if (get_wrapped_char(grid, row, col + i, height, width) != words[w][i])
                        match = 0;
                }
                if (match)
                    mark_word_in_grid(used, row, col, length, height, width, 0, 1);

                match = 1;
                for (int i = 0; i < length && match; i++)
                {
                    if (get_wrapped_char(grid, row, col - i, height, width) != words[w][i])
                        match = 0;
                }
                if (match)
                    mark_word_in_grid(used, row, col, length, height, width, 0, -1);
            }
        }
    }

    for (int col = 0; col < width; col++)
    {
        for (int row = 0; row < height; row++)
        {
            for (int w = 0; w < word_count; w++)
            {
                int length = strlen(words[w]);

                int match = 1;
                for (int i = 0; i < length && match; i++)
                {
                    if (row + i >= height || grid[row + i][col] != words[w][i])
                        match = 0;
                }
                if (match)
                    mark_word_in_grid(used, row, col, length, height, width, 1, 0);

                match = 1;
                for (int i = 0; i < length && match; i++)
                {
                    if (row - i < 0 || grid[row - i][col] != words[w][i])
                        match = 0;
                }
                if (match)
                    mark_word_in_grid(used, row, col, length, height, width, -1, 0);
            }
        }
    }

    int total = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            total += used[i][j];

    return total;
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

    char words[MAX_WORDS * 2][MAX_WORD_LEN];
    int word_count = load_words(file, words);

    char grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
    int width;
    int height = read_grid(file, grid, &width);
    fclose(file);

    if (height == 0 || width == 0)
    {
        fprintf(stderr, "Error: Invalid grid dimensions\n");
        return EXIT_FAILURE;
    }

    printf("Part 3: %d\n", count_runic_symbols(grid, words, word_count, height, width));
    return 0;
}
