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


char **cmd_split(char *cmd)
{
	int i = 0;
    char *token;
	char **command = malloc(sizeof(char*) * 1024);
	token= strtok(cmd, " ");
		while(token)
		{
            command[i] = token;
			token = strtok(NULL, " ");
            i++;
		}
        command[i] = NULL;
	return (command);
    //free(command);
}


int main(void)
{
    
    int i = 0;
        char *buffer, *token;;
        char **splited;
        size_t buff_size;
        size_t nums;
        pid_t pid;

     /**   
      * int i = 0;
    char *buffer, *token;;
    char **splited;
    size_t buff_size;
    size_t nums;
      * 
      * 
      * 
      *     write(STDIN_FILENO, "[dias@Archiso]~$: ", 19);
      nums = getline(&buffer, &buff_size, stdin);
        buffer[nums - 1] = '\0';
        splited = malloc(sizeof(char *) * (strlen(buffer) + 1));
        token = strtok(buffer, " ");
        while (token)
        {
            splited[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        splited[i] = NULL;

       /**
        *  while (*splited)
        * {
        *    printf("%s\n", *splited);
        *    splited++;
        * }
        */
       while (1)
       {
        printf("[dias@Archiso]~$: ");
        nums = getline(&buffer, &buff_size, stdin);
        buffer[nums - 1] = '\0';
        splited = calloc((strlen(buffer) + 1), sizeof(char*));
        token = strtok(buffer, " ");
        while (token)
        {
            splited[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        splited[i] = NULL;
        i = 0;
        
        pid = fork();
        if (pid == -1)
		{
			perror("Error forking !\n");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
        {
			if (execve(splited[0], splited, NULL) == -1)
			{	perror("shell");
                exit(EXIT_FAILURE);
            }
            
        }
            //exit(EXIT_SUCCESS);
       }
       wait(&pid);
       write(STDIN_FILENO, "[dias@Archiso]~$: ", 19);
       
       
}