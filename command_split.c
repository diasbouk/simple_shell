#include "shell.h"
    
/**
 * command_spiltter-Function to split command argumnets
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
return (full_command);
}
