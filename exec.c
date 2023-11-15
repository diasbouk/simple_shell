#include "shell.h"

/**
 * exec - Function to execute one command
 * @command: array of strings
 * @argv: argv
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