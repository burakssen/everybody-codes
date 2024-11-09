#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

// Function to convert string to uppercase
void to_upper(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Function to load words from the "WORDS:" line in the file
int load_words(FILE *file, char words[MAX_WORDS][MAX_WORD_LEN])
{
    char line[MAX_LINE];
    int word_count = 0;

    if (fgets(line, sizeof(line), file))
    {
        char *word_list = strchr(line, ':');
        if (word_list)
        {
            word_list++; // Skip past the colon
            char *word = strtok(word_list, ",\n");
            while (word && word_count < MAX_WORDS)
            {
                while (*word == ' ')
                    word++; // Trim leading spaces
                strncpy(words[word_count], word, MAX_WORD_LEN - 1);
                words[word_count][MAX_WORD_LEN - 1] = '\0';
                to_upper(words[word_count]);
                word_count++;
                word = strtok(NULL, ",\n");
            }
        }
    }

    return word_count;
}

// Function to count occurrences of runic words in the line
int count_runic_words(const char *line, char words[MAX_WORDS][MAX_WORD_LEN], int word_count)
{
    int total_runic_words = 0;
    int line_len = strlen(line);

    for (int i = 0; i < line_len; i++)
    {
        for (int j = 0; j < word_count; j++)
        {
            int word_len = strlen(words[j]);
            if (i + word_len <= line_len && strncmp(&line[i], words[j], word_len) == 0)
            {
                total_runic_words++;
            }
        }
    }

    return total_runic_words;
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

    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = load_words(file, words);

    // Skip the empty line
    char line[MAX_LINE];
    fgets(line, sizeof(line), file);

    // Process the text line
    int total_runic_words = 0;
    if (fgets(line, sizeof(line), file))
    {
        to_upper(line);
        total_runic_words = count_runic_words(line, words, word_count);
    }

    fclose(file);
    printf("Part 1: %d\n", total_runic_words);
    return 0;
}
