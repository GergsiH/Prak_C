#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>

#define BLUE "\e[1;36m"
#define R "\e[m"


int ERRORS = 0;
int flag_EOF = 0;
int flag_quotes = 0;
int count = 0;
int double_quotes = 0;
int file = 0;
int new_line = 0;

const char double_symbols[5] = "&|<>\0";
const char special_symbols[4] = ";()\0";

int cd_check(char **arr) 
{
    if (!strcmp(arr[0], "cd")) 
    {
        if (arr[1] == NULL) 
        {
            chdir(getenv("HOME"));
        } 
        else if ((arr[2] != NULL) || chdir(arr[1])) 
        {
            printf("ERROR: you can't change the directory\n");
            return -1;
        }
        return 1;
    }
    return 0;
}

char *get_word(FILE *file_a)
{
    int word_a = 0;
    int symb = 0;
    char c = 0;
    int count_word = 0;
    char *word = (char*)malloc(sizeof(char));
    word[count_word] = 0;

    while ((c = getc(file_a)) != EOF) /*enter the words*/
    {
        if (flag_quotes) /*checking for double quotes*/
        {
            if (c == '\n') /*new line - quotes are not closed, error*/
            {
                new_line = 1;
                flag_quotes = 0;
                fprintf(stderr, "ERROR: Odd number of quotation marks\n");
                ERRORS = 1;
                free(word);
                return NULL;
            }
            if (c == '"') /*quotes are closed - we reset the flag*/
            {
                flag_quotes = 0;
            }
            else /*not closed - add a c to the word*/
            {
                word = (char *)realloc(word, (count_word + 2) * sizeof(char));
                word[count_word] = c;
                count_word++;
            }
        }
        else if (symb) /*checking for a special symbols*/
        {
            symb = 0;
            if (c == word[count_word - 1]) /*if double - add to the array*/
            {
                word = (char *)realloc(word, (count_word + 2) * sizeof(char));
                word[count_word] = c;
                count_word++;
                word[count_word] = 0;
                return word;
            }
            else /*if they are single, we return them to the input*/
            {
                ungetc(c, file_a);
                word[count_word] = 0;
                return word;
            }
        }
        else if (isspace(c)) /*checking for whitespace characters*/
        {
            if (word_a) /*if the word does exist, then...*/
            {
                if (c == '\n') /*line count*/
                {
                    new_line = 1;
                }
                word[count_word] = 0;
                return word;
            }
            else /*if there is no word, then...*/
            {
                if (c == '\n')
                {
                    new_line = 1;
                    free(word);
                    word = NULL;
                    return NULL;
                }
                continue;
            }
        }
        else if (c == '"')
        {
            word_a = 1;
            flag_quotes = 1;
        }
        else if (strchr(double_symbols, c)) /*searches for the c character in the string*/
        {
            if (word_a)/*if a word - complete the word*/
            {
                ungetc(c, file_a);
                word[count_word] = 0;
                return word;
            }
            else/*otherwise, we add a symbol to the word*/
            {
                symb = 1;
                word = (char *) realloc(word, (count_word + 2) * sizeof (char));
                word[count_word] = c;
                count_word++;
            }
        }
        else if (strchr(special_symbols, c))/*checking for single symbols*/
        {
            if (word_a)
            {
                ungetc(c, file_a);
                word[count_word] = 0;
                return word;
            }
            else
            {
                word = (char*) realloc(word,(count_word + 2) * sizeof(char));
                word[0] = c;
                word[1] = '\0';
                return word;
            }
        }
        else 
        {
            word_a = 1;
            word = (char *) realloc (word, (count_word + 2) * sizeof(char));
            word[count_word] = c;
            count_word++;
        }
    }
    flag_EOF = 1;
    if (word != NULL)
    {
        free(word);
        return NULL;
    }
    else
    {
        word[count_word] = 0;
        return word;
    }
}

char **add_elem_to_array (char **arr, char *word)
{
    arr = realloc(arr, (count + 2) * sizeof(char*));
    arr[count] = word;
    count++;
    arr[count] = NULL;
    return arr;
}

void free_array(char** arr)
{
    while(*arr != NULL)
    {
        free(*arr);
        arr++;
    }
}


int main(int argc, char **argv)
{
    FILE *file_a;
    file_a = stdin;
    char *word;
    char **words_array;

    if (argc == 2)
    {
        file_a = fopen(argv[1], "r");
        file = 1;
    }

    while (!flag_EOF)
    {
        if(!file)
        {
            printf("%s> %s", BLUE, R);
        }

        count = 0;
        words_array = malloc(sizeof(char*));
        new_line = 0;
        ERRORS = 0;

        while (!flag_EOF && !new_line && !ERRORS)
        {
            word = get_word(file_a);
            words_array = add_elem_to_array(words_array, word);
        }
        if (ERRORS)
        {
            free_array(words_array);
            free(words_array);
            continue;
        }
        if (!flag_EOF && (*words_array != NULL))
        {
            if ((cd_check(words_array)) == 0)
            {
                pid_t pid = fork();
                if (pid == -1)
                {
                    perror("ERROR: System error");
                    exit(1);
                }
                else if (pid == 0)
                {
                    execvp(words_array[0], words_array);
                    perror("ERROR");
                    exit(2);
                }
                wait(0);
            }
        }
        free_array(words_array);
        free(words_array);
    }
    if (argc == 2)
    {
        fclose(file_a);
    } 
    printf("\n");
    return 0;
}