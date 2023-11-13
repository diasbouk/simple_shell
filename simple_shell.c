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
