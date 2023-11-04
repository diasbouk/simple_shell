#include "shell.h"

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
		while (1)
		{
			printf("[Dias@Archiso]~$: ");
			buffer_size = getline(&buffer,&buffer_size, stdin);
			buffer[buffer_size - 1] = '\0';
			commands = strtok(buffer, " ");
			if (execve(commands, command, NULL) == -1)
				printf("Error !\n");
		}
	return (0);
}
