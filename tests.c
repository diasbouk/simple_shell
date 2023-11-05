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
	int i = 1;
	char **command = calloc(sizeof(char), strlen(cmd));
	command[0] = strtok(cmd, " ");
		while(command[i])
		{
			command[i] = strtok(NULL, " ");
			i++;
		}
	return (command);
}
/////////////////////////////////////////////////

int main(void)
{
	int index = 0, n_chars;
	pid_t pid;
	pid_t id = getpid();
	char *buffer;
	size_t buffer_size;
	
	while (index < 5)
	{
		write(STDIN_FILENO, "[dias@Archiso]~$: ", 19);

		n_chars = getline(&buffer, &buffer_size, stdin);
		char *cmd = strtok(buffer, " \t\n");
		char *arguments[] = {cmd};
		printf("%s\n", cmd);
		printf("%s\n", arguments[0]);
		
			pid = fork();
		if (pid == -1)
		{
			puts("Error forking !\n");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
			if (execve(arguments[0], arguments, NULL) == -1)
				perror("Error\n");
		index++;
	}
	wait(&pid);
		return (0);
}