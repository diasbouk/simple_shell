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

int main(void)
{
	int i  = 0;
	char *str = NULL;
	size_t strsize;
	int nums = getline(&str,&strsize, stdin);
	if (nums = -1)
		exit(EXIT_FAILURE);
	find_file("/bin/ls");
	find_file("ls");
	find_file("/bin/clear");
	char **test = (char **)malloc(sizeof(char *) * strlen(str) + 1);
	if (test == NULL)
		perror("ERROR ALLOCATING !");
		exit(EXIT_FAILURE);
	test = cmd_split(str);
	free(str);
	/** 
	  	while(test[i])
	  	{
	 		 printf("%s\n", test[i]);
	   		i++;
	  	}
	 */
	//printf("%s\n", getenv("PWD"));
	//printf("%s\n", _getenv("PWD"));
	while (test[i])
	{
		free(test[i]);
		test[i] = NULL;
		i++;
	}
	free(test);
	test = NULL;
		return (0);
}