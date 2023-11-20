#include "shell.h"

/**
 * handle_command - command into pathname
 * @cmd: command to transform
 * Return: new command
 */
char *handle_command(char *cmd)
{
    struct stat st;
    char *token;
    char *path = _getenv("PATH");
    char *full_cmd = NULL;
        if (stat(cmd, &st) == 0)
            return (cmd);
        token = strtok(path, ":");
        while (token)
        {
            full_cmd = NULL;
            full_cmd = realloc(full_cmd, sizeof(char) * (strlen(token) + strlen(cmd) + 2));
            strcpy(full_cmd, token);
            strcat(full_cmd, "/");
            strcat(full_cmd, cmd);

            if (stat(full_cmd, &st) == 0)
                return (full_cmd);


            token = strtok(NULL, ":");
            free(full_cmd);
            full_cmd = NULL;
        }

        return (NULL);
}

