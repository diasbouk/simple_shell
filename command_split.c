#include "shell.h"

/**
 * cmd_split - function to split command into separated strings
 * @str: command to split
 * Return: AN array of strings from command
*/

char **cmd_split(char *str)
{
int i = 0, count = 0;
char  *temp = malloc(sizeof(char) * (strlen(str) + 1));
char **splited;
char *token;
_strcpy(temp, str);
token = strtok(temp, DELIM);
if (!token)
{
free(str);
str = NULL;
free(temp);
temp = NULL;
}
while (token != NULL)
{
count++;
token = strtok(NULL, DELIM);
}
splited = malloc(sizeof(char *) * (count + 1));
token = strtok(str, DELIM);
while (token)
{
splited[i] = strdup(token);
token = strtok(NULL, " ");
i++;
}
splited[i] = NULL;
free(str), str = NULL;
free(temp), temp = NULL;
return (splited);
}
