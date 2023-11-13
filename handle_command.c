#include "shell.h"

/**
 * handle_command - command into pathname
 * @cmd: command to transform
 * Return: new command
 */
char * handle_command(char *cmd)
{
  struct stat st;    
    char *token;
    char *path;
    char *temp;

    if (cmd == NULL)
        return (NULL); 

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

