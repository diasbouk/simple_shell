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

/**
 * handle_command - command into pathname
 * @cmd: command to transform
 * Return: new command
 * */
void handle_command(char *cmd)
{
    if (strstr(cmd, "/bin/ls") == NULL)
        return;
    else
        cmd = strcat("/bin/ls", cmd);
}

/**
 * _free_t - free a 2D array
 * @arr: array to free;
*/
void _free_t(char **arr)
{
    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    
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
        if (isatty(STDIN_FILENO) == 1)
        write(STDOUT_FILENO, "[yassine_dias@shell]: ~$ ", 26);
        num_of_chars = getline(&buffer, &buffer_size, stdin);
        if (num_of_chars == -1)
            {
                if(isatty(STDIN_FILENO) == 1)
                    write(STDOUT_FILENO, "\n", 2);
                free(buffer);
                return (NULL);
            }
        buffer[num_of_chars - 1] = '\0';
        return (buffer);
        free(buffer);
}

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
        
    }
    
    
}

/**
 * exec - Function to execute one command
 * @command: array of strings
 * @argv: argv
 * Return: exit status
*/
int exec(char **command,const char **argv, char **envp)
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        if (execve(command[0], command, envp) == -1)
        {
            perror(argv[0]);
            _free_t(command);
            exit(status);
        }
    }
    else
    {
        wait(&pid);
        _free_t(command);
    }
    return (status);
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
    pid_t pid;
    
        (void)argc;
        (void)argv;

        while (1)
        {
            argm = get_line();
            if (argm == NULL)
                return (status);
            command = cmd_split(argm);
            exec(command, argv, envp);
        }
            return 0;
}
