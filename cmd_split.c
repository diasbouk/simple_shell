#include "shell.h"

/*
 * cmd_split - splits a command into a list of arguments
 *
 *
 * Return: An array of strings
 */
char **cmd_split(char *cmd)
{
	char **command = NULL;
	*command = strtok(cmd, " ");
		while(*(*command))
		{
			*command = strtok(NULL, " ");
			*(command++);
		}
	return (command);
}
