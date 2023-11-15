#include "shell.h"

/**
 * _getenv - gets the value of a given env_var
 * @name: variable name
 * Return: Pointer to its value
*/
char *_getenv(const char *name)
{
    int i = 0;
    extern char **environ;
    while (environ[i])
    {
        if (strncmp(environ[i], name, strlen(name)) == 0)
        {
            return ((strstr(environ[i], "=")) + 1);
        }
        i++;
    }
    return (NULL);
}