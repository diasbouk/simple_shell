#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

struct  stat st;



int find_file(char *file)
{
    char *token;
    token = strtok(file, " \t\n");
	if (stat(file, &st) == 0)
        return (0);
    else
        return (1);
}
/////////////////////////////////////////////////


char **cmd_split(char *str)
{
	int i = 0;
	char **splited = malloc((strlen(str) + 1) * sizeof(char*));
	char *token = strtok(str, " ");
        while (token)
        {
            splited[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        splited[i] = NULL;
		return(splited);
}

/**
 * special_str - str for path
 * @str: string to edit
 * @ch: char to find
 * Return: pointer to ch or NULL .
*/
char *special_str(char *str, char ch)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == ch)
        {
            return (&str[i]);
            break;
        }
        i++;
    }
    
}

/**
 * get_line - get a command 
 * Return: pointer to cmd if success
 * -> NULL if not
*/

char *get_line(void)
{
    char *buffer = NULL, *command;
    size_t buffer_size;
    int num_of_chars;
        if (isatty(STDIN_FILENO) == 1)
        write(STDOUT_FILENO, "[yassine_dias@shell]: ~$ ", 26);
        num_of_chars = getline(&buffer, &buffer_size, stdin);
        if (num_of_chars == -1)
            {
                if(isatty(STDIN_FILENO) == 1)
                    write(STDOUT_FILENO, "\n", 2);
                free(buffer);
                return (NULL);
            }
        buffer[num_of_chars - 1] = '\0';
        return (buffer);
        free(buffer);
}


/**
 * _strstr - find needle in haystack
 * @haystack: String that contains substring
 * @needle: Subrstring to be located .
 * Return: pointer to needle if its found
 * NULL if not .
 */
char *_strstr(char *haystack, char *needle)
{
while (*haystack != '\0')
{
while ((*haystack == *needle && *needle != '\0') || !*needle)
{
return (haystack);
}
haystack++;
}
if (*needle == '\0')
return (haystack);
return (0);
}



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
            //break;
        }
        i++;
    }
}


/**
 * handle_command - command into pathname
 * @cmd: command to transform
 * Return: new command
 * */
char * handle_command(char *cmd)
{
    char *token;
    char *path;
    char *temp;

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
        path = _strstr(full_cmd, "/");
        if (stat(path, &st) == 0)
        {
            free(full_cmd);
            return (path);
            break;
        }
        else
        {
            path = NULL;
        }
        token = strtok(NULL, ":");
        free(full_cmd);
    }
    free(cmd);
    if (path == NULL)
        return (NULL);
    
}


int main(int ac, char **argv)
{
   // str1 = _getenv("HOME");
    // find_file("hsh");
    char *buffer;
    extern char **environ;
    int n;
    int i = 0;
   size_t buff;
        n = getline(&buffer, &buff, stdin);
        if (n == 1)
            exit(WEXITSTATUS(0));
        buffer[n - 1] = '\0';
        const char *path = handle_command(buffer);
        if (path != NULL)
            printf("%s : FOUND\n", path);
        else
            printf("%s : NOT FOUND\n", path);
        free(buffer);
		return (0);
}



// HANDLE COMMAND BACKUP
// !! DO NOT TOUCH THIS
/*

char * handle_command(char *cmd)
{
    char *token;
    char *full_cmd = NULL;
    token = strtok(_getenv("PATH"), ":");
    while (token)
    {
        full_cmd = realloc(full_cmd, (sizeof(char) * (strlen(cmd) + strlen(token) + 2)));
    if (full_cmd == NULL)
        return;
    strcat(full_cmd, token);
    printf("\n\n%s\n", full_cmd);
    if (find_file(cmd) == 0)
    {
        printf("%s\n", cmd);
        return (cmd);
    }
    if (find_file(cmd) == 1)
    {
        strcat(full_cmd, "/");
        strcat(full_cmd, cmd);
        printf("%s\n", full_cmd);
    }
    free(full_cmd);
        token = strtok(NULL, ":");
    } 
}

*/