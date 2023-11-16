#include "shell.h"


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
