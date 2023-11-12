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

struct  stat st;

/**
 * _getenv - gets the value of a given env_var
 * @name: variable name
 * Return: Pointer to its value
*/
char *_getenv(const char *name, char **envp)
{
    int i = 0;
    while (envp[i])
    {
        if (strncmp(envp[i], name, strlen(name)) == 0)
        {
            return ((strstr(envp[i], "=")) + 1);
            break;
        }
        i++;
    }
    return (NULL);
}

/**
 * find_file - finds info about file
 * @file: file path name
 * prints FOUND if its found and NOT FOUND if not
*/
void find_file(char *file)
{
	if (stat(file, &st) == 0)
		printf("%s: FOUND\n", file);
	else
		printf("%s: NOT FOUND\n", file);
	
}

/**
 * cmd_split - function to split command into separated strings
 * @str: command to split
 * Return: AN array of strings from command
*/

char **cmd_split(char *str)
{
	int i = 0;
	char **splited = malloc((strlen(str) + 1) * sizeof(char*));
	char *token = strtok(str, " ");
        while (token)
        {
            splited[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        splited[i] = NULL;
		return(splited);
        i = 0;
        while (splited[i])
        {
            free(splited[i]);
        }
        
        free(splited);
}

/**
 * main - Main function of sh
 * Return: 0 on success
*/
int main(int ac, char **av, char **env)
{
    
        int i = 0;
        char *buffer = NULL;
        char **splited = NULL;
        size_t buff_size = 0;
        int nums;
        pid_t pid;

        (void)ac;
        (void)av;
       while (1)
       {
        if (isatty(STDIN_FILENO) == 1)
            write(STDIN_FILENO, "[dias@Archiso]~$: ", 19);
        nums = getline(&buffer, &buff_size, stdin);
        if (nums == -1)
        {
            perror("ERROR BUFFER !");
        }
        buffer[nums - 1] = '\0';
        splited = malloc((strlen(buffer) + 1) * sizeof(char*));
        splited = cmd_split(buffer);
        pid = fork();
        if (pid == -1)
		{
			perror("Error forking !\n");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
        {
			if (execve(splited[0], splited, env) == -1)
			{	perror("shell");
                exit(EXIT_FAILURE);
            }
        if (splited != NULL)
        {
            while (splited[i])
                {
                    free(splited[i]);
                }
                free(splited);
        }
        if (buffer != NULL)
            free(buffer);
        }
        else
            wait(&pid);
       
       }
       
}



char ** cmd_split(char *str)
{
	char *token = NULL, *temp = NULL;
    char **command = NULL;
    int count = 0, i = 0;
    if (!str)
        return (NULL);
    temp = strdup(str);
    token = strtok(temp, DELIM);
    while (token)
    {
        count++;
        token = strtok(NULL, DELIM); 
    }
    free(temp);
    temp = NULL;

    command = malloc(sizeof(char *) * (count + 1));
    if (!command)
    {
        return (NULL);
        free(str);
        str = NULL;
    }
    token = strtok(str, DELIM);

    while (token)
    {
        command[i] = token;
        token = strtok(NULL, DELIM); 
        i++;
    }
    command[i] = NULL;
    return(command);
    free(str);
    str = NULL;
}