#include "shell.h"

/**
 * get_line - get a command
 * Return: pointer to cmd if success
 * -> NULL if not
*/

char *get_line(void)
{
char *buffer = NULL;
size_t buffer_size;
int num_of_chars;
if (isatty(STDIN_FILENO))
{
write(STDOUT_FILENO, "[yassine_dias@shell]: ~$ ", 26);
}
num_of_chars = getline(&buffer, &buffer_size, stdin);
if (num_of_chars == -1)
{
free(buffer);
buffer = NULL;
return (NULL);
}
buffer[num_of_chars - 1] = '\0';
fflush(STDIN_FILENO);
return (buffer);
}
