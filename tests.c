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


void find_file(char *file)
{
	if (stat(file, &st) == 0)
		printf("%s: FOUND\n", file);
	else
		printf("%s: NOT FOUND\n", file);
	
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
 * _getenv - gets the value of a given env_var
 * @name: variable name
 * Return: Pointer to its value
*/
char *_getenv(const char *name)
{
    int i = 0;
    char **envp;
    while (envp[i])
    {
        if (strncmp(envp[i], name, strlen(name)) == 0)
        {
            return ((strstr(envp[i], "=")) + 1);
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
 * handle_command - command into pathname
 * @cmd: command to transform
 * Return: new command
 * */
void handle_command(char *cmd)
{
    if (strstr(cmd, "/bin/ls") == NULL)
        cmd = strcat("/bin/ls", cmd);
    else
        return;
}


int main(int ac, char **argv, char **envp)
{
    char *argm;
    char **command;
    int status = 0;
    pid_t pid;(void)ac;

	while (1)
    {
        argm = get_line();
        pid = fork();
        command = cmd_split(argm);
        if (pid == 0)
        {
            if (execve(command[0], command, envp) ==-1)
                perror(argv[0]);
        }
    }
    
		return (0);
}