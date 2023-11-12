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
#define BUFFER_SIZE 0
#define BUFF_NUM 0
#define DELIM " \t\n"

struct stat st;


/**
 * *_strcpy - cp
 * @dest: dest
 * @src: string
 *
 * Return: the pointer to dest
 */
char *_strcpy(char *dest, char *src)
{
	int rtg, n;
		rtg = 0;
		while (src[rtg] != '\0')
		{
			rtg++;
		}

		for (n = 0; n < rtg; n++)
		{
			dest[n] = src[n];
		}
		dest[n] = '\0';

			return (dest);
}
/**
 * _strlen - check the code
 * @s: parameter string
 * Return: Always 0.
 */
int _strlen(char *s)
{
	int length;
		length = 0;
		while (s[length])
		{
			length++;
		}
		return (length);
}

/**
 * get_line - function to read input from STDIN
 * Return: pointer to buffer or NULL if it failed
*/

char *get_line()
{
    char *buffer;
    size_t buffer_size;
    int num, status;
    if (isatty(STDERR_FILENO))
        write(STDERR_FILENO, "$ ", 2);
    num = getline(&buffer, &buffer_size, stdin);
    if (num == -1)
    {
        free(buffer);
        buffer = NULL;
        exit(WEXITSTATUS(status));
    }
    buffer[num - 1] = '\0';
    return (buffer);
}


/**
 * main - main func
 * @argv: argv
 * @ac: argc
 * @envp: environnement vars
 * Return: 0 or an 
*/
int main(int ac, char **argv, char **envp)
{
    char *buff;
    while (1)
    {
        buff = get_line();
        if (buff != NULL)
        {
            printf("%s\n", buff);
        }
    }
    free(buff);
    buff = NULL;
}