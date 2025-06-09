#define _POSIX_C_SOURCE 200809L/*for getline function*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>/*for getline function*/
#include <string.h>

enum return_type {SUCCESS, INVALID_NUMBER_OF_ARGUMENTS, FOPEN_FAILURE, DELETION_ERROR};
 
struct command_handlers
{
    char *string;
    int (*comparison_function)(const char *string, const char *special_input);
    int (*operation_function)(char *);
};

int string_find(const char *string, const char *special_input);
int count_lines(char *filename);
int remove_file(char *filename);
int exit_program();
int beginning_of_string(char *filename, char *str);
int beginning_of_string_wrapper(char *filename);

char *str = NULL;

int main(int argc, char *argv[])
{
    char *filename;

    FILE *f;

    size_t buffer = 0; 
   
    ssize_t length;
   
    struct command_handlers handler[] = {
    {"-remove", string_find, remove_file},
    {"-count", string_find, count_lines},
    {"-exit", string_find, exit_program},    
    {"<", string_find, beginning_of_string_wrapper}
    }; 

    if (argc != 2)
    {
        printf("The proper use is ./a.out <name_of_file>\n");
        return INVALID_NUMBER_OF_ARGUMENTS;
    }

    filename = argv[1];

    while (1)
    {
        printf("Enter a string: ");

        length = getline(&str, &buffer, stdin);  

        assert(NULL != str);

        assert(length != -1);

     
        if (handler[0].comparison_function(str, handler[0].string))
            return (handler[0].operation_function(filename));

        if (handler[1].comparison_function(str, handler[1].string))
        {
            handler[1].operation_function(filename);
            continue;/*To avoid printing actual command to terminal*/
        }

        if (handler[2].comparison_function(str, handler[2].string))
            return (handler[2].operation_function(filename));

        if (handler[3].comparison_function(str, handler[3].string))
        {
            handler[3].operation_function(filename);
            continue;/*To avoid printing actual command to terminal*/
        }
        

        f = fopen(filename, "a");
 
        if (f == NULL)
        {
            perror("fopen failed");
            return FOPEN_FAILURE;
        }

        fprintf(f, "%s", str);

        fclose(f);
     }

     return SUCCESS;
}

int string_find(const char *string, const char *special_input)
{
    int result;

    if (strstr(string, special_input) != NULL)
        result = 1;

    else
        result = 0;

    return result;
}

int count_lines(char *filename)
{
    FILE *f;
    int count;
    int ch;
   
    f = fopen(filename, "r");

    if (f == NULL)
    {
        perror("fopen failed");
        return FOPEN_FAILURE;
    }

    count = 0;

    while ((ch = getc(f)) != EOF)
    {
        if (ch == '\n')
        {
            ++count;
        }
    }

    fclose(f);
    printf("Total lines: %d\n", count);

    return SUCCESS;
}

int remove_file(char *filename)
{
    if (remove(filename) == 0)
    {
        printf("The file %s was deleted.\n", filename);
    }
    else
    {
        printf("Error deleting the file %s.\n", filename);
            return DELETION_ERROR;
    }
    return SUCCESS;
}

int exit_program()
{
    printf("Exiting program.\n");
    return SUCCESS;
}

int beginning_of_string(char *filename, char *str)
{
    FILE *tmp_file;
    FILE *original_file;
    char buffer[BUFSIZ];
    size_t n;

    original_file = fopen(filename, "r");
    if (!original_file)
    {
        perror("Failed to open the original file");
        return FOPEN_FAILURE;
    }

    tmp_file = fopen("tmpfile.tmp", "w");
    if (!tmp_file)
    {
        perror("Failed to open the temporary file");
        fclose(original_file);
        return FOPEN_FAILURE;
    }

    fprintf(tmp_file, "%s", str + 1);
    
    while ((n = fread(buffer, 1, sizeof(buffer), original_file)) > 0)
    {
        fwrite(buffer, 1, n, tmp_file);
    }

    fclose(original_file);
    fclose(tmp_file);

    remove(filename);
    rename("tmpfile.tmp", filename);

    return SUCCESS;
}

int beginning_of_string_wrapper(char *filename)
{
    return beginning_of_string(filename, str);
}
