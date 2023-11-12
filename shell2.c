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

struct stat st;


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
 * _getenv - gets the value of a given env_var
 * @name: variable name
 * Return: Pointer to its value
*/
char *_getenv(const char *name)
{
    int i = 0;
    extern char **environ;
    while (environ[i])
    {
        if (strncmp(environ[i], name, strlen(name)) == 0)
        {
            return ((strstr(environ[i], "=")) + 1);
            //break;
        }
        i++;
    }
}


/**
 * handle_command - command into pathname
 * @cmd: command to transform
 * Return: new command
 * */
char * handle_command(char *cmd)
{
    char *token;
    char *path;
    char *temp;

    if (stat(cmd, &st) == 0)
    {
        return(cmd);
    }

    
    token = strtok(_getenv("PATH"), ":");
    while (token)
    {
        char *full_cmd = malloc(sizeof(char) * (strlen(cmd) + strlen(token)));
        strcat(full_cmd, token);
        strcat(full_cmd, "/");
        strcat(full_cmd, cmd);
        temp = strdup(full_cmd);
        path = _strstr(temp, "/");
        if (stat(path, &st) == 0)
        {
            free(full_cmd);
            fflush(STDIN_FILENO);
            return (path);
            free(temp);
            break;
        }
        else
        {
            path = NULL;
        }
        token = strtok(NULL, ":");
        free(full_cmd);
    }
    free(temp);
    free(cmd);
    if (path == NULL)
        return (NULL);
    
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
        write(STDOUT_FILENO, "[yassine_dias@shell]: ~$ ", 26);
        num_of_chars = getline(&buffer, &buffer_size, stdin);
        if (num_of_chars == -1)
            {
                free(buffer);
                buffer = NULL;
                fflush(STDIN_FILENO);
                return (NULL);
                
            }
        buffer[num_of_chars - 1] = '\0';
        fflush(STDIN_FILENO);
        return (buffer);
}

/**
 * cmd_split - function to split command into separated strings
 * @str: command to split
 * Return: AN array of strings from command
*/

char ** cmd_split(char *str)
{
	int i = 0, count = 0;
    char  *temp = malloc(sizeof(char) * (strlen(str) + 1));
	char **splited;
	char *token;
    _strcpy(temp, str);
    token = strtok(temp, DELIM);
    while (token != NULL)
    {
        count++;
        token = strtok(NULL, DELIM);
    }

    splited = malloc(sizeof(char *) * (count + 1));
    token = strtok(str, DELIM);
        while (token)
        {
            splited[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        splited[i] = NULL;

		return(splited);
        // no free after return
        // put free before
        free(splited);
        free(str);
        str = NULL;
        free(temp);
        temp = NULL;
}

/**
 * exec - Function to execute one command
 * @command: array of strings
 * @argv: argv
 * Return: exit status
*/
int exec(char **command,const char **argv, char **envp)
{
    pid_t forked;
    int status;
    forked = fork();
    if (forked == 0)
    {
        if (execve(command[0], command, envp) == -1)
        {   
            perror(command[0]);
            free(command);
            command = NULL;
            exit(0);
        }
    }
    else
    {
        waitpid(forked, &status, 0);   
        free(command);     
    }
    return (WEXITSTATUS(status));
}

/**
 * main - main function
 * @argc: PAR
 * @argv: PAR2
 * Return: o if success or -1 if not
*/
int main(int argc, char const **argv, char **envp)
{
    char *argm;
    char **command;
    int status = 0;    
        (void)argc;
        (void)argv;

        while (1)
        {
            argm = get_line();
            if (argm == NULL)
            {
                if(isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 2);
                return (status);
                exit(WEXITSTATUS(status));
            }
            command = cmd_split(argm);
            if (command == NULL)
                continue;
            command[0] = handle_command(command[0]);

            //status = exec(command, argv, envp);

            pid_t forked;
            int status;
            forked = fork();
            if (forked == 0)
            {
                if (execve(command[0], command, envp) == -1)
                {   
                    perror(command[0]);
                    free(command);
                    command = NULL;
                    exit(0);
                }
            }
            else
            {
                waitpid(forked, &status, 0);   
                free(command);     
            }
        }
            return 0;
}
