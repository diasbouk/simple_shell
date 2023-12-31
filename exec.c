#include "shell.h"

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
