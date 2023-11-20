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
#define BUFFER_SIZE 0
#define BUFF_NUM 0
#define DELIM " \t\n"
extern char **environ;

/**
 * *_strcpy - cp
 * @dest: dest
 * @src: string
 *
 * Return: the pointer to dest
 */
char *_strcpy(char *dest, char *src)
{
	int rtg, n;
		rtg = 0;
		while (src[rtg] != '\0')
		{
			rtg++;
		}

		for (n = 0; n < rtg; n++)
		{
			dest[n] = src[n];
		}
		dest[n] = '\0';

			return (dest);
}
/**
 * _strlen - check the code
 * @s: parameter string
 * Return: Always 0.
 */
int _strlen(char *s)
{
	int length;
		length = 0;
		while (s[length])
		{
			length++;
		}
		return (length);
}


/**
 * _strstr - find needle in haystack
 * @haystack: String that contains substring
 * @needle: Subrstring to be located .
 * Return: pointer to needle if its found
 * NULL if not .
 */
char *_strstr(char *haystack, char *needle)
{
while (*haystack != '\0')
{
while ((*haystack == *needle && *needle != '\0') || !*needle)
{
return (haystack);
}
haystack++;
}
if (*needle == '\0')
return (haystack);
return (0);
}


/**
 * _free_t - free a 2D array
 * @arr: array to free;
*/
void _free_t(char **arr)
{
int i = 0;
if (!arr)
return;
while (arr[i] != NULL)
{
free(arr[i]);
arr[i] = NULL;
i++;
}
free(arr);
arr = NULL;
}

/**
 * _getenv - gets the value of a given env_var
 * @name: variable name
 * Return: Pointer to its value
*/
char *_getenv(const char *name)
{
    int i = 0;
        while (environ[i])
        {
            if (strncmp(environ[i], name, strlen(name)) == 0)
                return ((strstr(environ[i], "=")) + 1);
            i++;
        }
    return (NULL);
}

void print_env(char **envi)
{
int i = 0;
while(envi[i])
{
printf("%s", envi[i]);
i++;
}
}



/**
 * handle_path - handles path
 * @arg: first arguments
 * Return: new arg
*/

char *handle_path(char *arg)
{
    struct stat st;
    char *full_cmd = NULL, *token = NULL, *path = NULL, *clone = NULL;

        path = _getenv("PATH");

        if (stat(arg, &st) == 0)
            return (arg);
        
        token = strtok(path, ":");
        
        while (token)
        {
            full_cmd = realloc(full_cmd, sizeof(char) * (strlen(token) + strlen(arg) + 2));
            
            strcpy(full_cmd, token);
            
            strcat(full_cmd, "/");
            
            strcat(full_cmd, arg);
            

            if (stat(full_cmd, &st) == 0)
                return (full_cmd);
        
            free(full_cmd);
            full_cmd = NULL;
            token = strtok(NULL, ":");
            
        }
        
        return(NULL);
}


/**
 * command_splitter - Function to split command argumnets
 * @command: command enterde by user
 * Return: An array of strings containing args . 
*/

char **command_spiltter(char *command)
{
    char *token = NULL, **full_command = NULL, *temp_cmd = NULL;
    int i = 0, count  = 0;

    temp_cmd = malloc(sizeof(char) * (_strlen(command) + 1));
    _strcpy(temp_cmd, command);
    token = strtok(temp_cmd, DELIM);
    if (!token)
    {
        free(temp_cmd);
        return (NULL);
    }
    while (token)
    {
        count++;
        token = strtok(NULL, DELIM);
    }
    free(temp_cmd), temp_cmd = NULL;

    full_command = malloc(sizeof(char *) * (count + 1));
    token = strtok(command, DELIM);
    if (!token)
    {
        free(full_command);
        return (NULL);
    }
    while (token)
    {
        full_command[i] = strdup(token);
        token = strtok(NULL, DELIM); 
        i++;
    }
    full_command[i] = NULL;
    return(full_command);
}

/**
 * _exec_it - Function to execute one command
 * @command: array of strings
 * @envp: argv
 * Return: exit status
*/
int _exec_it(char **command, char **envp)
{
    pid_t forked;
    int status;

    forked = fork();
    if (forked == 0)
    {
        if (execve(command[0], command, envp) == -1)
        {
            perror(command[0]);
            _free_t(command);
            command = NULL;
        }
    }
    else
    {
        waitpid(forked, &status, 0);
        _free_t(command);
    }
    return (WEXITSTATUS(status));
}



/**
 * main -  Main func
 * @argc: Arg count
 * @argv:  argv 
 * @envp: environnement vars
 * Return: 0 on success or another number .
*/
int main(int argc, char **argv, char **envp)
{
    char *buffer = NULL, **new_args = NULL;
    size_t buffer_size;
    int num_of_chars;

    (void)argc;
    (void)argv;

    while (1)
    {


        if (isatty(STDIN_FILENO))
            write(STDIN_FILENO, "dias@yassine$ : ", 17);
        num_of_chars = getline(&buffer, &buffer_size, stdin);
        if (num_of_chars == -1)
        {
            free(buffer);
            exit(0);
        }
        buffer[num_of_chars - 1] = '\0';
        
        if (strcmp(buffer, "exit") == 0)
        {
            free(buffer);
            exit(0);
        }

        new_args = command_spiltter(buffer);
        
        if (new_args == NULL)
        {
            free(buffer);
            return(0);
        }
        
        new_args[0] = handle_path(new_args[0]);
        if (new_args[0])
            _exec_it(new_args, envp);

    }

    return (0);
    

}