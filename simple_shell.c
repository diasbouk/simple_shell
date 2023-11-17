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
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 2);
return (status);
}
command = cmd_split(argm);
if (command == NULL)
{
status  = 0;
continue;
}
if (strcmp(command[0], "exit") == 0)
{
write(STDIN_FILENO, "exit\n", 6);
if (command[1])
{
status = atoi(command[1]);
_free_t(command);
exit(status);
}
else
{
_free_t(command);
exit(WEXITSTATUS(status));
}
}
if (strcmp(command[0], "env") == 0)
{
_free_t(command);
print_env(envp);
exit(WEXITSTATUS(status));
}
status = _exec_it(command, envp);
}
return (0);
}
