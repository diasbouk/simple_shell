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

/**
 * cmd_split - check ...
 * 
*/


/**
 * main - main function of dir
 * 
*/
int main()
{
	char *buffer = NULL; 
	size_t buffer_size;
	char *commands;
	char **command;
	//	while (1)
	//
				int i = 0;
				int child_pid = fork();
				if (child_pid == 0)
				{
				 
				}
				else
				{
					wait(&child_pid);
					while(i < 4)
					{
						puts("[Dias@Archiso]~$: ");
						buffer_size = getline(&buffer,&buffer_size, stdin);
						buffer[buffer_size - 1] = '\0';
						commands = strtok(buffer, " ");
						execve(commands, command, NULL);
					}
					printf("Error !\n");
				}
	//	}
	return (0);
}
