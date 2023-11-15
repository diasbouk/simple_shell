#include "shell.h"

/**
 * main - main function for simple_shell .
 * @argc: args count .
 * @argv: arguments passed when running .
 * @envp: environnement variables .
 * Return: 0 on success or another integer .
 */
int main(int argc, char **argv, char **envp)
{
    char *argm;
    char **command;
    int status = 0;

    pid_t forked;
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
            {
              fflush(STDIN_FILENO);
              continue;
            }
            if (strcmp(command[0], "exit") == 0)
              exit(0);
            command[0] = handle_command(command[0]);
            
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
