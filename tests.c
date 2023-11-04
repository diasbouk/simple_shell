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
	char *command[strlen(cmd)];
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
	char str[] = "ls ~/ -al\n";
	char **res = cmd_split(str);
	printf("%s", res[0]);
	return (0);
}
