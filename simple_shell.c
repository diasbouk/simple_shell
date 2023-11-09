#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

/**
 * cmd_split - function to split command into separated strings
 * @str: command to split
 * Return: AN array of strings from command
*/

char ** cmd_split(char *str)
{
	char *token = NULL;
    char *dup = NULL;
    char **command = NULL;
    int i = 0, count = 0;
    dup = strdup(str);
    token = strtok(dup, " \t");
    while (token)
    {
        count++;
        token = strtok(NULL, " \t");
    }
    free(dup);
    command = malloc(sizeof(char *) * count + 1);
    token = strtok(str, " \t");
    while (token)
    {
        command[i] = token;
        token = strtok(NULL, " \t");
        i++;
    }
    command[i] = NULL;
    return(command);
}

/**
 * get_line - get a command 
 * Return: pointer to cmd if success
 * -> NULL if not
*/

char *get_line(void)
{
    char *buffer;
	size_t buffer_size;
	size_t chars_num;
	if (isatty(STDIN_FILENO) == 1)
		write(STDOUT_FILENO, "[yassine_dias@shell]: ~$ ", 26);
	chars_num = getline(&buffer, &buffer_size, stdin);
	if (chars_num == -1)
            {
                if(isatty(STDIN_FILENO) == 1)
                    write(STDOUT_FILENO, "\n", 2);
                free(buffer);
                return (NULL);
            }
        buffer[chars_num - 1] = '\0';
        return (buffer);
        free(buffer);
}

int main(int argc, char **argv, char **envp)
{
	char *buffer, **command;
	int i = 0;
	buffer = get_line();

	if (buffer = NULL) /*if CTRL + D or EOF*/
		exit(0);
		printf("%s\n", buffer);
	
	command = cmd_split(buffer);
	while (command[i])
	{
		printf("%s\n", command[i]);
	}
	
	return (0);
}