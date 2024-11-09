#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

void to_upper(char *str)
{
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

void reverse_string(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int load_words(FILE *file, char words[MAX_WORDS][MAX_WORD_LEN], char reversed_words[MAX_WORDS][MAX_WORD_LEN])
{
    char line[MAX_LINE];
    int word_count = 0;
    if (fgets(line, sizeof(line), file))
    {
        char *word_list = strchr(line, ':');
        if (word_list)
        {
            for (char *word = strtok(++word_list, ",\n"); word && word_count < MAX_WORDS; word = strtok(NULL, ",\n"))
            {
                while (*word == ' ')
                    word++;
                strncpy(words[word_count], word, MAX_WORD_LEN - 1);
                words[word_count][MAX_WORD_LEN - 1] = '\0';
                to_upper(words[word_count]);
                strncpy(reversed_words[word_count], words[word_count], MAX_WORD_LEN);
                reverse_string(reversed_words[word_count]);
                word_count++;
            }
        }
    }
    return word_count;
}

int count_runic_symbols_in_line(const char *line, char words[MAX_WORDS][MAX_WORD_LEN], char reversed_words[MAX_WORDS][MAX_WORD_LEN], int word_count)
{
    int line_len = strlen(line), total_symbols = 0;
    int *symbol_counts = calloc(line_len, sizeof(int));
    if (!symbol_counts)
        return 0;

    for (int direction = 0; direction < 2; direction++)
    {
        char(*current_words)[MAX_WORD_LEN] = direction ? reversed_words : words;
        for (int i = 0; i < line_len; i++)
        {
            for (int j = 0; j < word_count; j++)
            {
                int word_len = strlen(current_words[j]);
                if (i + word_len <= line_len && strncmp(&line[i], current_words[j], word_len) == 0)
                {
                    for (int k = 0; k < word_len; k++)
                        symbol_counts[i + k] = 1;
                }
            }
        }
    }

    for (int i = 0; i < line_len; i++)
        total_symbols += symbol_counts[i];

    free(symbol_counts);
    return total_symbols;
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
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char words[MAX_WORDS][MAX_WORD_LEN], reversed_words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = load_words(file, words, reversed_words);
    char empty_line[MAX_LINE];
    fgets(empty_line, sizeof(char[MAX_LINE]), file); // Skip empty line after "WORDS:"

    int total_runic_symbols = 0;
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;
        if (*line)
        {
            to_upper(line);
            total_runic_symbols += count_runic_symbols_in_line(line, words, reversed_words, word_count);
        }
    }

    fclose(file);
    printf("Part 2: %d\n", total_runic_symbols);
    return 0;
}
