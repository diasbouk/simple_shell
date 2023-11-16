#ifndef SIMPLE_SHELL
#define SIMPLE_SHELL
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
extern char **environ;
char **cmd_split(char *cmd);
char *_strcpy(char *dest, char *src);
int _strlen(char *s);
char *_strstr(char *haystack, char *needle);
char *_getenv(const char *name);
char * handle_command(char *cmd);
void _free_t(char **arr);
char *get_line(void);
int _exec_it(char **command, char **envp);
#endif
