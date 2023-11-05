#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

#define BUFFERSIZE 1024
#define TOK_BUFFERSIZE 128
#define TOK_DELIM " \t\r\n\a"

extern char **environ;

typedef struct data
{
	char **av;
	char *user_in;
	char **args;
	int status;
	int ln_count;
	char **var_environ;
	char *pid;
} Data_sl;

typedef struct sperator_lst_s
{
	char separator;
	struct sperator_lst_s *next;
} sperator_lst;

typedef struct LineList
{
	char *line;
	struct LineList *next;
} list_ln;

typedef struct StoreVar_list
{
	int len_var;
	char *val;
	int len_val;
	struct StoreVar_list *next;
} StoreVar;

typedef struct builtin_s
{
	char *cmd_name;
	int (*f)(Data_sl *Data_sh);
} builtin_s;

sperator_lst *add_node(sperator_lst **head, char sep);
void free_sperator_lst(sperator_lst **head);
list_ln *AddLineNode(list_ln **head, char *line);
void free_list_ln(list_ln **head);
StoreVar *add_Storevar(StoreVar **head, int lenvar, char *var, int lenval);

void free_StoreVar(StoreVar **head);
char *_strcat(char *dest, const char *src);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);
char *_strchr(char *s, char c);

int _strspn(char *s, char *autorise);
void _memcpy(void *dis_ptr, const void *ptr, unsigned int size);
void *func_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char **func_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size);
char *_strdup(const char *s);

int _strlen(const char *s);
int cmp_chars(char str[], const char *delim);
char *_strtok(char str[], const char *delim);
int _isdigit(const char *s);
void rev_str(char *s);

int rep_char(char *user_in, int i);
int find_error(char *user_in, int i, char last);
int first_char(char *user_in, int *i);
void print_error(Data_sl *Data_sh, char *user_in, int i, int bool);
int check_error(Data_sl *Data_sh, char *user_in);

char *cls_comment(char *in);
void shell_loop(Data_sl *Data_sh);
char *read_line(int *End_of);
char *swap_char(char *user_in, int bool);
void add_nodes(sperator_lst **head_s, list_ln **head_l, char *user_in);

void go_next(sperator_lst **list_s, list_ln **list_l, Data_sl *Data_sh);
int cmd_spliter(Data_sl *Data_sh, char *user_in);
char **line_spliter(char *user_in);
void check_env(StoreVar **h, char *in, Data_sl *data);
int check_vars(StoreVar **h, char *in, char *st, Data_sl *data);

char *replaced_user_in(StoreVar **head, char *user_in, char *new_user_in, int nlen);
char *rep_var(char *user_in, Data_sl *Data_sh);
void bring_line(char **lineptr, size_t *n, char *buffer, size_t j);
ssize_t get_line(char **lineptr, size_t *n, FILE *stream);
int exec_line(Data_sl *Data_sh);

int is_cdir(char *path, int *i);
char *_which(char *cmd, char **var_environ);
int exe_cmd(Data_sl *Data_sh);
int check_error_cmd(char *dir, Data_sl *Data_sh);
int cmd_exec(Data_sl *Data_sh);

char *get_env(const char *cmd_name, char **var_environ);
int _env(Data_sl *Data_sh);
char *copy_info(char *cmd_name, char *value);
void envSet(char *cmd_name, char *value, Data_sl *Data_sh);
int env_Set(Data_sl *Data_sh);

int _unset_env(Data_sl *Data_sh);
void cd_dot(Data_sl *Data_sh);
void cd_to(Data_sl *Data_sh);
void cd_prev(Data_sl *Data_sh);
void cd_home(Data_sl *Data_sh);


int cd_shell(Data_sl *Data_sh);
int (*get_builtin(char *cmd))(Data_sl *Data_sh);
int exit_shell(Data_sl *Data_sh);
int get_len(int n);
char *func_itoa(int n);

int _atoi(char *s);
char *strcat_cd(Data_sl *, char *, char *, char *);
char *error_get_cd(Data_sl *Data_sh);
char *error_not_found(Data_sl *Data_sh);
char *error_exit_shell(Data_sl *Data_sh);

char *error_get_alias(char **args);
char *error_env(Data_sl *Data_sh);
char *error_syntax(char **args);
char *error_permission(char **args);
char *error_path(Data_sl *Data_sh);


int get_error(Data_sl *Data_sh, int eval);
void get_sigint(int sig);
void c_env(void);
void func_helper_set_env(void);
void func_helper_unset_env(void);

void func_helper_general(void);
void func_helper_exit(void);
void func_helper(void);
void func_helper_alias(void);
void func_helper_cd(void);

int msg_help(Data_sl *Data_sh);

/**
 * strcat_cd - function concatenates error
 *
 * @Data_sh: data (directory)
 * @msg: message
 * @error: stdout msg
 * @ver_str: ln_count lines
 * Return: error
 */
char *strcat_cd(Data_sl *Data_sh, char *msg, char *error, char *ver_str)
{
	char *NoPermited_flag;

	_strcpy(error, Data_sh->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, Data_sh->args[0]);
	_strcat(error, msg);
	if (Data_sh->args[1][0] == '-')
	{
		NoPermited_flag = malloc(3);
		NoPermited_flag[0] = '-';
		NoPermited_flag[1] = Data_sh->args[1][1];
		NoPermited_flag[2] = '\0';
		_strcat(error, NoPermited_flag);
		free(NoPermited_flag);
	}
	else
	{
		_strcat(error, Data_sh->args[1]);
	}

	_strcat(error, "\n");
	_strcat(error, "\0");
	return (error);
}

/**
 * error_get_cd - error message to cmd get_cd
 * @Data_sh: data directory
 * Return: Error
 */
char *error_get_cd(Data_sl *Data_sh)
{
	int lent, idLent;
	char *error, *ver_str, *msg;

	ver_str = func_itoa(Data_sh->ln_count);
	if (Data_sh->args[1][0] == '-')
	{
		msg = ": Illegal option ";
		idLent = 2;
	}
	else
	{
		msg = ": can't cd to ";
		idLent = _strlen(Data_sh->args[1]);
	}

	lent = _strlen(Data_sh->av[0]) + _strlen(Data_sh->args[0]);
	lent += _strlen(ver_str) + _strlen(msg) + idLent + 5;
	error = malloc(sizeof(char) * (lent + 1));

	if (error == 0)
	{
		free(ver_str);
		return (NULL);
	}

	error = strcat_cd(Data_sh, msg, error, ver_str);

	free(ver_str);

	return (error);
}

/**
 * error_not_found - generic error command not found
 * @Data_sh: data (ln_count, arguments)
 * Return: Error
 */
char *error_not_found(Data_sl *Data_sh)
{
	int lent;
	char *error;
	char *ver_str;

	ver_str = func_itoa(Data_sh->ln_count);
	lent = _strlen(Data_sh->av[0]) + _strlen(ver_str);
	lent += _strlen(Data_sh->args[0]) + 16;
	error = malloc(sizeof(char) * (lent + 1));
	if (error == 0)
	{
		free(error);
		free(ver_str);
		return (NULL);
	}
	_strcpy(error, Data_sh->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, Data_sh->args[0]);
	_strcat(error, ": not found\n");
	_strcat(error, "\0");
	free(ver_str);
	return (error);
}

/**
 * error_exit_shell - generic error exit
 * @Data_sh: data (ln_count, arguments)
 *
 * Return: Error
 */
char *error_exit_shell(Data_sl *Data_sh)
{
	int lent;
	char *error;
	char *ver_str;

	ver_str = func_itoa(Data_sh->ln_count);
	lent = _strlen(Data_sh->av[0]) + _strlen(ver_str);
	lent += _strlen(Data_sh->args[0]) + _strlen(Data_sh->args[1]) + 23;
	error = malloc(sizeof(char) * (lent + 1));
	if (error == 0)
	{
		free(ver_str);
		return (NULL);
	}
	_strcpy(error, Data_sh->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, Data_sh->args[0]);
	_strcat(error, ": Illegal number: ");
	_strcat(error, Data_sh->args[1]);
	_strcat(error, "\n\0");
	free(ver_str);

	return (error);
}

/**
 * error_env - error message to get_env.
 * @Data_sh: data (ln_count, arguments)
 * Return: error.
 */
char *error_env(Data_sl *Data_sh)
{
	int lent;
	char *error;
	char *ver_str;
	char *msg;

	ver_str = func_itoa(Data_sh->ln_count);
	msg = ": Unable to add/remove from environment\n";
	lent = _strlen(Data_sh->av[0]) + _strlen(ver_str);
	lent += _strlen(Data_sh->args[0]) + _strlen(msg) + 4;
	error = malloc(sizeof(char) * (lent + 1));
	if (error == 0)
	{
		free(error);
		free(ver_str);
		return (NULL);
	}

	_strcpy(error, Data_sh->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, Data_sh->args[0]);
	_strcat(error, msg);
	_strcat(error, "\0");
	free(ver_str);

	return (error);
}
/**
 * error_path - error path permission denied
 * @Data_sh: data (ln_count, arguments).
 * Return: error.
 */
char *error_path(Data_sl *Data_sh)
{
	int lent;
	char *ver_str;
	char *error;

	ver_str = func_itoa(Data_sh->ln_count);
	lent = _strlen(Data_sh->av[0]) + _strlen(ver_str);
	lent += _strlen(Data_sh->args[0]) + 24;
	error = malloc(sizeof(char) * (lent + 1));
	if (error == 0)
	{
		free(error);
		free(ver_str);
		return (NULL);
	}
	_strcpy(error, Data_sh->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, Data_sh->args[0]);
	_strcat(error, ": Permission denied\n");
	_strcat(error, "\0");
	free(ver_str);
	return (error);
}

/**
 * func_helper_env - Helper for the builtin env
 * Return: void
 */
void func_helper_env(void)
{
	char *help = "env: env [option] [cmd_name=value] [command [args]]\n\t";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "Print the enviroment of the shell.\n";
	write(STDOUT_FILENO, help, _strlen(help));

}
/**
 * func_helper_set_env - Help for builtin setenv
 * Return: void
 */
void func_helper_set_env(void)
{

	char *help = "setenv: setenv (const char *cmd_name, const char *value,";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "int replace)\n\t";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "Add a new definition to the environment\n";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * func_helper_unset_env - Help for unset_env
 * Return: void
 */
void func_helper_unset_env(void)
{
	char *help = "unset_env: unset_env (const char *cmd_name)\n\t";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "Remove an entry completely from the environment\n";
	write(STDOUT_FILENO, help, _strlen(help));
}


/**
 * func_helper_general - for help builtin
 * Return: void
 */
void func_helper_general(void)
{
	char *help = "$ bash, version 1.0(1)-release\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "These commands are defined internally.Type 'help' to see the list";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "Type 'help cmd_name' to find out more about the function 'cmd_name'.\n\n ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = " alias: alias [cmd_name=['str']]\n cd: cd [-L|[-P [-e]] [-@]] ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "[dir]\nexit: exit [n]\n  env: env [option] [cmd_name=value] [command ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "[args]]\n  setenv: setenv [variable] [value]\n  unset_env: ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "unset_env [variable]\n";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * func_helper_exit - Help for builint exit
 * Return: void
 */
void func_helper_exit(void)
{
	char *help = "exit: exit [n]\n Exit shell.\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "Exits the shell with a status of N. If N is ommited, the exit";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "statusis that of the last command executed\n";
	write(STDOUT_FILENO, help, _strlen(help));
}

/**
 * func_helper - for builtin help.
 * Return: void
 */
void func_helper(void)
{
	char *help = "help: help [-dms] [pattern ...]\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "\tDisplay information about builtin commands.\n ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "Displays brief summaries of builtin commands.\n";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * func_helper_alias - for the builtin alias.
 * Return: void
 */
void func_helper_alias(void)
{
	char *help = "alias: alias [-p] [cmd_name[=value]...]\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "\tDefine or display aliases.\n ";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * func_helper_cd - for the builtin alias.
 * Return: void
 */
void func_helper_cd(void)
{
	char *help = "cd: cd [-L|[-P [-e]] [-@]] [dir]\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "\tChange the shell working directory.\n ";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * add_node - adds a separator at the end
 * of a sperator_lst.
 * @head: head
 * @sep: separator (; | &).
 * Return: address of the head.
 */
sperator_lst *add_node(sperator_lst **head, char sep)
{
	sperator_lst *new, *tmp;

	new = malloc(sizeof(sperator_lst));
	if (new == NULL)
		return (NULL);

	new->separator = sep;
	new->next = NULL;
	tmp = *head;

	if (tmp == NULL)
	{
		*head = new;
	}
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}

	return (*head);
}

/**
 * free_sperator_lst - frees a sperator_lst
 * @head: head of the linked list.
 * Return: void.
 */
void free_sperator_lst(sperator_lst **head)
{
	sperator_lst *tmp;
	sperator_lst *curr;

	if (head != NULL)
	{
		curr = *head;
		while ((tmp = curr) != NULL)
		{
			curr = curr->next;
			free(tmp);
		}
		*head = NULL;
	}
}

/**
 * AddLineNode - adds a cmd line at the end
 * of list_ln.
 * @head: head.
 * @line: cmd line.
 * Return: head adresse
 */
list_ln *AddLineNode(list_ln **head, char *line)
{
	list_ln *new, *tmp;

	new = malloc(sizeof(list_ln));
	if (new == NULL)
		return (NULL);

	new->line = line;
	new->next = NULL;
	tmp = *head;

	if (tmp == NULL)
	{
		*head = new;
	}
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}

	return (*head);
}

/**
 * free_list_ln - frees list_ln
 * @head: head link list.
 * Return: void.
 */
void free_list_ln(list_ln **head)
{
	list_ln *tmp;
	list_ln *curr;

	if (head != NULL)
	{
		curr = *head;
		while ((tmp = curr) != NULL)
		{
			curr = curr->next;
			free(tmp);
		}
		*head = NULL;
	}
}


/**
 * add_Storevar - adds a variable at the end
 * StoreVar list.
 * @head: head
 * @lenvar: var length
 * @val: value of the variable.
 * @lenval: length of the value.
 * Return: address of the head.
 */
StoreVar *add_Storevar(StoreVar **head, int lenvar, char *val, int lenval)
{
	StoreVar *new, *tmp;

	new = malloc(sizeof(StoreVar));
	if (new == NULL)
		return (NULL);

	new->len_var = lenvar;
	new->val = val;
	new->len_val = lenval;

	new->next = NULL;
	tmp = *head;

	if (tmp == NULL)
	{
		*head = new;
	}
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}

	return (*head);
}

/**
 * free_StoreVar - frees StoreVar list
 * @head: head.
 * Return: void.
 */
void free_StoreVar(StoreVar **head)
{
	StoreVar *tmp;
	StoreVar *curr;

	if (head != NULL)
	{
		curr = *head;
		while ((tmp = curr) != NULL)
		{
			curr = curr->next;
			free(tmp);
		}
		*head = NULL;
	}
}


/**
 * _memcpy - copies between void pointers.
 * @dis_ptr: destination pointer.
 * @ptr: source pointer.
 * @size: size of the new pointer.
 *
 * Return: void.
 */
void _memcpy(void *dis_ptr, const void *ptr, unsigned int size)
{
	char *char_ptr = (char *)ptr;
	char *char_dis_ptr = (char *)dis_ptr;
	unsigned int i;

	for (i = 0; i < size; i++)
		char_dis_ptr[i] = char_ptr[i];
}

/**
 * func_realloc - reallocates a memory block.
 * @ptr: pointer to the memory previously allocated.
 * @old_size: size, in bytes, of the allocated space of ptr.
 * @new_size: new size, in bytes, of the new memory block.
 *
 * Return: ptr.
 * if new_size == old_size, returns ptr.
 * if malloc fails, NULL.
 */
void *func_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *dis_ptr;

	if (ptr == NULL)
		return (malloc(new_size));

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}

	if (new_size == old_size)
		return (ptr);

	dis_ptr = malloc(new_size);
	if (dis_ptr == NULL)
		return (NULL);

	if (new_size < old_size)
		_memcpy(dis_ptr, ptr, new_size);
	else
		_memcpy(dis_ptr, ptr, old_size);

	free(ptr);
	return (dis_ptr);
}

/**
 * func_reallocdp - reallocates block of a double pointer.
 * @ptr: pointer to the memory previously allocated.
 * @old_size: size, in bytes, allocation .
 * @new_size: new size, in bytes - new memory block.
 *
 * Return: ptr.
 * if new_size == old_size, returns ptr.
 * if malloc fails, NULL.
 */
char **func_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size)
{
	char **dis_ptr;
	unsigned int i;

	if (ptr == NULL)
		return (malloc(sizeof(char *) * new_size));

	if (new_size == old_size)
		return (ptr);

	dis_ptr = malloc(sizeof(char *) * new_size);
	if (dis_ptr == NULL)
		return (NULL);

	for (i = 0; i < old_size; i++)
		dis_ptr[i] = ptr[i];

	free(ptr);

	return (dis_ptr);
}


/**
 * get_len - check lent number.
 * @n: int.
 * Return: number lent.
 */
int get_len(int n)
{
	unsigned int n1;
	int lent = 1;

	if (n < 0)
	{
		lent++;
		n1 = n * -1;
	}
	else
	{
		n1 = n;
	}
	while (n1 > 9)
	{
		lent++;
		n1 = n1 / 10;
	}

	return (lent);
}
/**
 * func_itoa - converts int to str.
 * @n: int
 * Return: String.
 */
char *func_itoa(int n)
{
	unsigned int n1;
	int lent = get_len(n);
	char *buffer;

	buffer = malloc(sizeof(char) * (lent + 1));
	if (buffer == 0)
		return (NULL);

	*(buffer + lent) = '\0';

	if (n < 0)
	{
		n1 = n * -1;
		buffer[0] = '-';
	}
	else
	{
		n1 = n;
	}

	lent--;
	do {
		*(buffer + lent) = (n1 % 10) + '0';
		n1 = n1 / 10;
		lent--;
	}
	while (n1 > 0)
		;
	return (buffer);
}

/**
 * _atoi - converts str to integer.
 * @s: user_in str.
 * Return: integer.
 */
int _atoi(char *s)
{
	unsigned int count = 0, size = 0, oi = 0, pn = 1, m = 1, i;

	while (*(s + count) != '\0')
	{
		if (size > 0 && (*(s + count) < '0' || *(s + count) > '9'))
			break;

		if (*(s + count) == '-')
			pn *= -1;

		if ((*(s + count) >= '0') && (*(s + count) <= '9'))
		{
			if (size > 0)
				m *= 10;
			size++;
		}
		count++;
	}

	for (i = count - size; i < count; i++)
	{
		oi = oi + ((*(s + i) - 48) * m);
		m /= 10;
	}
	return (oi * pn);
}

/**
 * _strcat - concatenate two strs
 * @dest: destination
 * @src: source
 * Return: dest
 */
char *_strcat(char *dest, const char *src)
{
	int i;
	int j;

	for (i = 0; dest[i] != '\0'; i++)
		;

	for (j = 0; src[j] != '\0'; j++)
	{
		dest[i] = src[j];
		i++;
	}

	dest[i] = '\0';
	return (dest);
}
/**
 * *_strcpy - Copies the str pointed to by src.
 * @dest: destination
 * @src: source
 * Return: dest.
 */
char *_strcpy(char *dest, char *src)
{

	size_t a;

	for (a = 0; src[a] != '\0'; a++)
	{
		dest[a] = src[a];
	}
	dest[a] = '\0';

	return (dest);
}
/**
 * _strcmp - compares two strs.
 * @s1: str
 * @s2: str
 * Return: Always 0.
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	for (i = 0; s1[i] == s2[i] && s1[i]; i++)
		;

	if (s1[i] > s2[i])
		return (1);
	if (s1[i] < s2[i])
		return (-1);
	return (0);
}
/**
 * _strchr - locates a character into str,
 * @s: str.
 * @c: char.
 * Return: char.
 */
char *_strchr(char *s, char c)
{
	unsigned int i = 0;

	for (; *(s + i) != '\0'; i++)
		if (*(s + i) == c)
			return (s + i);
	if (*(s + i) == c)
		return (s + i);
	return ('\0');
}
/**
 * _strspn - gets the length of a prefix substr.
 * @s: initial segment.
 * @autorise: autoriseed bytes.
 * Return: count accepted bytes.
 */
int _strspn(char *s, char *autorise)
{
	int i, j, bool;

	for (i = 0; *(s + i) != '\0'; i++)
	{
		bool = 1;
		for (j = 0; *(autorise + j) != '\0'; j++)
		{
			if (*(s + i) == *(autorise + j))
			{
				bool = 0;
				break;
			}
		}
		if (bool == 1)
			break;
	}
	return (i);
}


/**
 * _strdup - duplicates a str in the heap.
 * @s: char pointer str
 * Return: duplicated
 */
char *_strdup(const char *s)
{
	char *new;
	size_t len;

	len = _strlen(s);
	new = malloc(sizeof(char) * (len + 1));
	if (new == NULL)
		return (NULL);
	_memcpy(new, s, len + 1);
	return (new);
}

/**
 * _strlen - lenght of a str.
 * @s: char pointer
 * Return: Always 0.
 */
int _strlen(const char *s)
{
	int len;

	for (len = 0; s[len] != 0; len++)
	{
	}
	return (len);
}

/**
 * cmp_chars - compare chars of strs
 * @str: user_in str.
 * @delim: delimiter.
 *
 * Return: 1 if true , 0 false.
 */
int cmp_chars(char str[], const char *delim)
{
	unsigned int i, j, k;

	for (i = 0, k = 0; str[i]; i++)
	{
		for (j = 0; delim[j]; j++)
		{
			if (str[i] == delim[j])
			{
				k++;
				break;
			}
		}
	}
	if (i == k)
		return (1);
	return (0);
}

/**
 * _strtok - splits a str delimiter.
 * @str: user_in str.
 * @delim: delimiter.
 *
 * Return: str.
 */
char *_strtok(char str[], const char *delim)
{
	static char *spltd, *str_end;
	char *str_start;
	unsigned int i, bool;

	if (str != NULL)
	{
		if (cmp_chars(str, delim))
			return (NULL);
		spltd = str;
		i = _strlen(str);
		str_end = &str[i];
	}
	str_start = spltd;
	if (str_start == str_end)
		return (NULL);

	for (bool = 0; *spltd; spltd++)
	{
		if (spltd != str_start)
			if (*spltd && *(spltd - 1) == '\0')
				break;
		/*Replacing delimiter for null char*/
		for (i = 0; delim[i]; i++)
		{
			if (*spltd == delim[i])
			{
				*spltd = '\0';
				if (spltd == str_start)
					str_start++;
				break;
			}
		}
		if (bool == 0 && *spltd) /*Str != Delim*/
			bool = 1;
	}
	if (bool == 0) /*Str == Delim*/
		return (NULL);
	return (str_start);
}

/**
 * _isdigit - defines if str is number
 * @s: user_in str
 * Return: 1 if a number.otherwise 0.
 */
int _isdigit(const char *s)
{
	unsigned int i;

	for (i = 0; s[i]; i++)
	{
		if (s[i] < 48 || s[i] > 57)
			return (0);
	}
	return (1);
}



/**
 * rev_str - reverses a str.
 * @s: user_in str.
 * Return: void.
 */
void rev_str(char *s)
{
	int count = 0, i, j;
	char *str, tmp;

	while (count >= 0)
	{
		if (s[count] == '\0')
			break;
		count++;
	}
	str = s;

	for (i = 0; i < (count - 1); i++)
	{
		for (j = i + 1; j > 0; j--)
		{
			tmp = *(str + j);
			*(str + j) = *(str + (j - 1));
			*(str + (j - 1)) = tmp;
		}
	}
}


/**
 * cd_shell - changes directory
 * @Data_sh: data
 * Return: 1 success
 */
int cd_shell(Data_sl *Data_sh)
{
	char *dir;
	int _home, _home2, i;

	dir = Data_sh->args[1];

	if (dir != NULL)
	{
		_home = _strcmp("$HOME", dir);
		_home2 = _strcmp("~", dir);
		i = _strcmp("--", dir);
	}

	if (dir == NULL || !_home || !_home2 || !i)
	{
		cd_home(Data_sh);
		return (1);
	}

	if (_strcmp("-", dir) == 0)
	{
		cd_prev(Data_sh);
		return (1);
	}

	if (_strcmp(".", dir) == 0 || _strcmp("..", dir) == 0)
	{
		cd_dot(Data_sh);
		return (1);
	}

	cd_to(Data_sh);

	return (1);
}


/**
 * cd_dot - changes directory
 * @Data_sh: data environ
 * Return: void
 */
void cd_dot(Data_sl *Data_sh)
{
	char pwd[PATH_MAX];
	char *dir, *cp_pwd, *cp_strtok_pwd;

	getcwd(pwd, sizeof(pwd));
	cp_pwd = _strdup(pwd);
	envSet("OLDPWD", cp_pwd, Data_sh);
	dir = Data_sh->args[1];
	if (_strcmp(".", dir) == 0)
	{
		envSet("PWD", cp_pwd, Data_sh);
		free(cp_pwd);
		return;
	}
	if (_strcmp("/", cp_pwd) == 0)
	{
		free(cp_pwd);
		return;
	}
	cp_strtok_pwd = cp_pwd;
	rev_str(cp_strtok_pwd);
	cp_strtok_pwd = _strtok(cp_strtok_pwd, "/");
	if (cp_strtok_pwd != NULL)
	{
		cp_strtok_pwd = _strtok(NULL, "\0");

		if (cp_strtok_pwd != NULL)
			rev_str(cp_strtok_pwd);
	}
	if (cp_strtok_pwd != NULL)
	{
		chdir(cp_strtok_pwd);
		envSet("PWD", cp_strtok_pwd, Data_sh);
	}
	else
	{
		chdir("/");
		envSet("PWD", "/", Data_sh);
	}
	Data_sh->status = 0;
	free(cp_pwd);
}

/**
 * cd_to - changes to a directory given
 * by the user
 *
 * @Data_sh: data relevant (directories)
 * Return: void
 */
void cd_to(Data_sl *Data_sh)
{
	char pwd[PATH_MAX];
	char *dir, *cp_pwd, *cp_dir;

	getcwd(pwd, sizeof(pwd));

	dir = Data_sh->args[1];
	if (chdir(dir) == -1)
	{
		get_error(Data_sh, 2);
		return;
	}

	cp_pwd = _strdup(pwd);
	envSet("OLDPWD", cp_pwd, Data_sh);

	cp_dir = _strdup(dir);
	envSet("PWD", cp_dir, Data_sh);

	free(cp_pwd);
	free(cp_dir);

	Data_sh->status = 0;

	chdir(dir);
}

/**
 * cd_prev - changes to the previous directory
 *
 * @Data_sh: data relevant (environ)
 * Return: void
 */
void cd_prev(Data_sl *Data_sh)
{
	char pwd[PATH_MAX];
	char *p_pwd, *p_oldpwd, *cp_pwd, *cp_oldpwd;

	getcwd(pwd, sizeof(pwd));
	cp_pwd = _strdup(pwd);

	p_oldpwd = get_env("OLDPWD", Data_sh->var_environ);

	if (p_oldpwd == NULL)
		cp_oldpwd = cp_pwd;
	else
		cp_oldpwd = _strdup(p_oldpwd);

	envSet("OLDPWD", cp_pwd, Data_sh);

	if (chdir(cp_oldpwd) == -1)
		envSet("PWD", cp_pwd, Data_sh);
	else
		envSet("PWD", cp_oldpwd, Data_sh);

	p_pwd = get_env("PWD", Data_sh->var_environ);

	write(STDOUT_FILENO, p_pwd, _strlen(p_pwd));
	write(STDOUT_FILENO, "\n", 1);

	free(cp_pwd);
	if (p_oldpwd)
		free(cp_oldpwd);

	Data_sh->status = 0;

	chdir(p_pwd);
}

/**
 * cd_home - changes directory
 * @Data_sh: environ
 * Return: void
 */
void cd_home(Data_sl *Data_sh)
{
	char *p_pwd, *home;
	char pwd[PATH_MAX];

	getcwd(pwd, sizeof(pwd));
	p_pwd = _strdup(pwd);

	home = get_env("HOME", Data_sh->var_environ);

	if (home == NULL)
	{
		envSet("OLDPWD", p_pwd, Data_sh);
		free(p_pwd);
		return;
	}

	if (chdir(home) == -1)
	{
		get_error(Data_sh, 2);
		free(p_pwd);
		return;
	}

	envSet("OLDPWD", p_pwd, Data_sh);
	envSet("PWD", home, Data_sh);
	free(p_pwd);
	Data_sh->status = 0;
}


/**
 * rep_char - counts the repetitions
 *
 * @user_in: user_in str
 * @i: index
 * Return: repetitions
 */
int rep_char(char *user_in, int i)
{
	if (*(user_in - 1) == *user_in)
		return (rep_char(user_in - 1, i + 1));

	return (i);
}

/**
 * find_error - finds syntax errors
 *
 * @user_in: user_in str
 * @i: index
 * @last: last char read
 * Return: error index. 0
 * errors
 */
int find_error(char *user_in, int i, char last)
{
	int count;

	count = 0;
	if (*user_in == '\0')
		return (0);

	if (*user_in == ' ' || *user_in == '\t')
		return (find_error(user_in + 1, i + 1, last));

	if (*user_in == ';')
		if (last == '|' || last == '&' || last == ';')
			return (i);

	if (*user_in == '|')
	{
		if (last == ';' || last == '&')
			return (i);

		if (last == '|')
		{
			count = rep_char(user_in, 0);
			if (count == 0 || count > 1)
				return (i);
		}
	}

	if (*user_in == '&')
	{
		if (last == ';' || last == '|')
			return (i);

		if (last == '&')
		{
			count = rep_char(user_in, 0);
			if (count == 0 || count > 1)
				return (i);
		}
	}

	return (find_error(user_in + 1, i + 1, *user_in));
}

/**
 * first_char - finds index first char
 *
 * @user_in: user_in str
 * @i: index
 * Return: 1 if there is an error.
 */
int first_char(char *user_in, int *i)
{

	for (*i = 0; user_in[*i]; *i += 1)
	{
		if (user_in[*i] == ' ' || user_in[*i] == '\t')
			continue;

		if (user_in[*i] == ';' || user_in[*i] == '|' || user_in[*i] == '&')
			return (-1);

		break;
	}

	return (0);
}

/**
 * print_error - prints error
 * @Data_sh: data strcture
 * @user_in: user_in str
 * @i: index of the error
 * @bool: control msg error
 * Return: void
 */
void print_error(Data_sl *Data_sh, char *user_in, int i, int bool)
{
	char *msg, *msg2, *msg3, *error, *count;
	int lent;

	if (user_in[i] == ';')
	{
		if (bool == 0)
			msg = (user_in[i + 1] == ';' ? ";;" : ";");
		else
			msg = (user_in[i - 1] == ';' ? ";;" : ";");
	}

	if (user_in[i] == '|')
		msg = (user_in[i + 1] == '|' ? "||" : "|");

	if (user_in[i] == '&')
		msg = (user_in[i + 1] == '&' ? "&&" : "&");

	msg2 = ": Syntax error: \"";
	msg3 = "\" unexpected\n";
	count = func_itoa(Data_sh->ln_count);
	lent = _strlen(Data_sh->av[0]) + _strlen(count);
	lent += _strlen(msg) + _strlen(msg2) + _strlen(msg3) + 2;

	error = malloc(sizeof(char) * (lent + 1));
	if (error == 0)
	{
		free(count);
		return;
	}
	_strcpy(error, Data_sh->av[0]);
	_strcat(error, ": ");
	_strcat(error, count);
	_strcat(error, msg2);
	_strcat(error, msg);
	_strcat(error, msg3);
	_strcat(error, "\0");

	write(STDERR_FILENO, error, lent);
	free(error);
	free(count);
}

/**
 * check_error - intermediate function to
 * find and print a syntax error
 *
 * @Data_sh: data strcture
 * @user_in: user_in str
 * Return: 1 if there is an error. 0 in other case
 */
int check_error(Data_sl *Data_sh, char *user_in)
{
	int begin = 0;
	int f_char = 0;
	int i = 0;

	f_char = first_char(user_in, &begin);
	if (f_char == -1)
	{
		print_error(Data_sh, user_in, begin, 0);
		return (1);
	}

	i = find_error(user_in + begin, 0, *(user_in + begin));
	if (i != 0)
	{
		print_error(Data_sh, user_in, begin + i, 1);
		return (1);
	}

	return (0);
}


/**
 * is_cdir - checks ":" if is in the current directory.
 * @path: type char pointer char.
 * @i: type int pointer of index.
 * Return: 1 if the path is searchable in the cd, 0 otherwise.
 */
int is_cdir(char *path, int *i)
{
	if (path[*i] == ':')
		return (1);

	while (path[*i] != ':' && path[*i])
	{
		*i += 1;
	}

	if (path[*i])
		*i += 1;

	return (0);
}

/**
 * _which - locates a command
 *
 * @cmd: command name
 * @var_environ: environment var
 * Return: location of the command.
 */
char *_which(char *cmd, char **var_environ)
{
	char *path, *ptr_path, *token_path, *dir;
	int len_dir, len_cmd, i;
	struct stat st;

	path = get_env("PATH", var_environ);
	if (path)
	{
		ptr_path = _strdup(path);
		len_cmd = _strlen(cmd);
		token_path = _strtok(ptr_path, ":");
		i = 0;
		while (token_path != NULL)
		{
			if (is_cdir(path, &i))
				if (stat(cmd, &st) == 0)
					return (cmd);
			len_dir = _strlen(token_path);
			dir = malloc(len_dir + len_cmd + 2);
			_strcpy(dir, token_path);
			_strcat(dir, "/");
			_strcat(dir, cmd);
			_strcat(dir, "\0");
			if (stat(dir, &st) == 0)
			{
				free(ptr_path);
				return (dir);
			}
			free(dir);
			token_path = _strtok(NULL, ":");
		}
		free(ptr_path);
		if (stat(cmd, &st) == 0)
			return (cmd);
		return (NULL);
	}
	if (cmd[0] == '/')
		if (stat(cmd, &st) == 0)
			return (cmd);
	return (NULL);
}

/**
 * exe_cmd - determines if is an executable
 *
 * @Data_sh: data strcture
 * Return: 0 if is not an executable, other number if it does
 */
int exe_cmd(Data_sl *Data_sh)
{
	struct stat st;
	int i;
	char *user_in;

	user_in = Data_sh->args[0];
	for (i = 0; user_in[i]; i++)
	{
		if (user_in[i] == '.')
		{
			if (user_in[i + 1] == '.')
				return (0);
			if (user_in[i + 1] == '/')
				continue;
			else
				break;
		}
		else if (user_in[i] == '/' && i != 0)
		{
			if (user_in[i + 1] == '.')
				continue;
			i++;
			break;
		}
		else
			break;
	}
	if (i == 0)
		return (0);

	if (stat(user_in + i, &st) == 0)
	{
		return (i);
	}
	get_error(Data_sh, 127);
	return (-1);
}

/**
 * check_error_cmd - verifies if user has permissions to access
 *
 * @dir: destination directory
 * @Data_sh: data strcture
 * Return: 1 if there is an error, 0 if not
 */
int check_error_cmd(char *dir, Data_sl *Data_sh)
{
	if (dir == NULL)
	{
		get_error(Data_sh, 127);
		return (1);
	}

	if (_strcmp(Data_sh->args[0], dir) != 0)
	{
		if (access(dir, X_OK) == -1)
		{
			get_error(Data_sh, 126);
			free(dir);
			return (1);
		}
		free(dir);
	}
	else
	{
		if (access(Data_sh->args[0], X_OK) == -1)
		{
			get_error(Data_sh, 126);
			return (1);
		}
	}

	return (0);
}

/**
 * cmd_exec - executes cmd lines
 *
 * @Data_sh: data relevant (args and user_in)
 * Return: 1 on success.
 */
int cmd_exec(Data_sl *Data_sh)
{
	pid_t pd;
	pid_t wpd;
	int state;
	int exec;
	char *dir;
	(void) wpd;

	exec = exe_cmd(Data_sh);
	if (exec == -1)
		return (1);
	if (exec == 0)
	{
		dir = _which(Data_sh->args[0], Data_sh->var_environ);
		if (check_error_cmd(dir, Data_sh) == 1)
			return (1);
	}

	pd = fork();
	if (pd == 0)
	{
		if (exec == 0)
			dir = _which(Data_sh->args[0], Data_sh->var_environ);
		else
			dir = Data_sh->args[0];
		execve(dir + exec, Data_sh->args, Data_sh->var_environ);
	}
	else if (pd < 0)
	{
		perror(Data_sh->av[0]);
		return (1);
	}
	else
	{
		do {
			wpd = waitpid(pd, &state, WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));
	}

	Data_sh->status = state / 256;
	return (1);
}


/**
 * cmp_env_cmd_name - compares env variables names
 * with the cmd_name passed.
 * @nenv: name of the environment var
 * @cmd_name: name passed
 *
 * Return: 0 if are not equal. Another value if they are.
 */
int cmp_env_cmd_name(const char *nenv, const char *cmd_name)
{
	int i;

	for (i = 0; nenv[i] != '='; i++)
	{
		if (nenv[i] != cmd_name[i])
		{
			return (0);
		}
	}

	return (i + 1);
}

/**
 * get_env - get an environment var
 * @cmd_name: name of the environment var
 * @var_environ: environment var
 *
 * Return: value of the environment var if is found.
 * In other case, returns NULL.
 */
char *get_env(const char *cmd_name, char **var_environ)
{
	char *ptr_env;
	int i, mov;

	/* Initialize ptr_env value */
	ptr_env = NULL;
	mov = 0;
	/* Compare all environment vars */
	/* environ is declared in the header file */
	for (i = 0; var_environ[i]; i++)
	{
		mov = cmp_env_cmd_name(var_environ[i], cmd_name);
		if (mov)
		{
			ptr_env = var_environ[i];
			break;
		}
	}

	return (ptr_env + mov);
}

/**
 * _env - prints the evironment variables
 *
 * @Data_sh: data relevant.
 * Return: 1 on success.
 */
int _env(Data_sl *Data_sh)
{
	int i, j;

	for (i = 0; Data_sh->var_environ[i]; i++)
	{

		for (j = 0; Data_sh->var_environ[i][j]; j++)
			;

		write(STDOUT_FILENO, Data_sh->var_environ[i], j);
		write(STDOUT_FILENO, "\n", 1);
	}
	Data_sh->status = 0;

	return (1);
}


/**
 * copy_info - copies info to create
 * a new env or alias
 * @cmd_name: name (env or alias)
 * @value: value (env or alias)
 * Return: new env or alias.
 */
char *copy_info(char *cmd_name, char *value)
{
	char *new;
	int len_cmd_name, len_value, len;

	len_cmd_name = _strlen(cmd_name);
	len_value = _strlen(value);
	len = len_cmd_name + len_value + 2;
	new = malloc(sizeof(char) * (len));
	_strcpy(new, cmd_name);
	_strcat(new, "=");
	_strcat(new, value);
	_strcat(new, "\0");

	return (new);
}

/**
 * envSet - sets an environment var
 * @cmd_name: name of the environment var
 * @value: value of the environment var
 * @Data_sh: data strcture (environ)
 * Return: void
 */
void envSet(char *cmd_name, char *value, Data_sl *Data_sh)
{
	int i;
	char *var_env, *cmd_name_env;

	for (i = 0; Data_sh->var_environ[i]; i++)
	{
		var_env = _strdup(Data_sh->var_environ[i]);
		cmd_name_env = _strtok(var_env, "=");
		if (_strcmp(cmd_name_env, cmd_name) == 0)
		{
			free(Data_sh->var_environ[i]);
			Data_sh->var_environ[i] = copy_info(cmd_name_env, value);
			free(var_env);
			return;
		}
		free(var_env);
	}

	Data_sh->var_environ = func_reallocdp(Data_sh->var_environ, i, sizeof(char *) * (i + 2));
	Data_sh->var_environ[i] = copy_info(cmd_name, value);
	Data_sh->var_environ[i + 1] = NULL;
}

/**
 * env_Set - compares env var names
 * with the name passed.
 * @Data_sh: data relevant (env cmd_name and env value)
 * Return: 1 on success.
 */
int env_Set(Data_sl *Data_sh)
{

	if (Data_sh->args[1] == NULL || Data_sh->args[2] == NULL)
	{
		get_error(Data_sh, -1);
		return (1);
	}

	envSet(Data_sh->args[1], Data_sh->args[2], Data_sh);

	return (1);
}

/**
 * _unset_env - deletes a environment var
 * @Data_sh: data relevant (env cmd_name)
 * Return: 1 on success.
 */
int _unset_env(Data_sl *Data_sh)
{
	char **reallocvar_environ;
	char *var_env, *cmd_name_env;
	int i, j, k;

	if (Data_sh->args[1] == NULL)
	{
		get_error(Data_sh, -1);
		return (1);
	}
	k = -1;
	for (i = 0; Data_sh->var_environ[i]; i++)
	{
		var_env = _strdup(Data_sh->var_environ[i]);
		cmd_name_env = _strtok(var_env, "=");
		if (_strcmp(cmd_name_env, Data_sh->args[1]) == 0)
		{
			k = i;
		}
		free(var_env);
	}
	if (k == -1)
	{
		get_error(Data_sh, -1);
		return (1);
	}
	reallocvar_environ = malloc(sizeof(char *) * (i));
	for (i = j = 0; Data_sh->var_environ[i]; i++)
	{
		if (i != k)
		{
			reallocvar_environ[j] = Data_sh->var_environ[i];
			j++;
		}
	}
	reallocvar_environ[j] = NULL;
	free(Data_sh->var_environ[k]);
	free(Data_sh->var_environ);
	Data_sh->var_environ = reallocvar_environ;
	return (1);
}


/**
 * exec_line - finds builtins and commands
 * @Data_sh: data relevant (args)
 * Return: 1 on success.
 */
int exec_line(Data_sl *Data_sh)
{
	int (*builtin)(Data_sl *Data_sh);

	if (Data_sh->args[0] == NULL)
		return (1);

	builtin = get_builtin(Data_sh->args[0]);

	if (builtin != NULL)
		return (builtin(Data_sh));

	return (cmd_exec(Data_sh));
}


/**
 * exit_shell - exits the shell
 * @Data_sh: data relevant (status and args)
 * Return: 0 on success.
 */
int exit_shell(Data_sl *Data_sh)
{
	unsigned int ustatus;
	int is_digit;
	int str_len;
	int big_number;

	if (Data_sh->args[1] != NULL)
	{
		ustatus = _atoi(Data_sh->args[1]);
		is_digit = _isdigit(Data_sh->args[1]);
		str_len = _strlen(Data_sh->args[1]);
		big_number = ustatus > (unsigned int)INT_MAX;
		if (!is_digit || str_len > 10 || big_number)
		{
			get_error(Data_sh, 2);
			Data_sh->status = 2;
			return (1);
		}
		Data_sh->status = (ustatus % 256);
	}
	return (0);
}


/**
 * get_builtin - builtin that pais the command in the arg
 * @cmd: command
 * Return: function pointer of the builtin command
 */
int (*get_builtin(char *cmd))(Data_sl *)
{
	builtin_s builtin[] = {
		{ "env", _env },
		{ "exit", exit_shell },
		{ "setenv", env_Set },
		{ "unset_env", _unset_env },
		{ "cd", cd_shell },
		{ "help", msg_help },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; builtin[i].cmd_name; i++)
	{
		if (_strcmp(builtin[i].cmd_name, cmd) == 0)
			break;
	}

	return (builtin[i].f);
}


/**
 * get_error - calls the error according the builtin, syntax or permission
 * @Data_sh: data strcture that contains arguments
 * @eval: error value
 * Return: error
 */
int get_error(Data_sl *Data_sh, int eval)
{
	char *error;

	switch (eval)
	{
	case -1:
		error = error_env(Data_sh);
		break;
	case 126:
		error = error_path(Data_sh);
		break;
	case 127:
		error = error_not_found(Data_sh);
		break;
	case 2:
		if (_strcmp("exit", Data_sh->args[0]) == 0)
			error = error_exit_shell(Data_sh);
		else if (_strcmp("cd", Data_sh->args[0]) == 0)
			error = error_get_cd(Data_sh);
		break;
	}

	if (error)
	{
		write(STDERR_FILENO, error, _strlen(error));
		free(error);
	}

	Data_sh->status = eval;
	return (eval);
}


/**
 * msg_help - function that retrieves help messages according builtin
 * @Data_sh: data strcture (args and user_in)
 * Return: Return 0
*/
int msg_help(Data_sl *Data_sh)
{

	if (Data_sh->args[1] == 0)
		func_helper_general();
	else if (_strcmp(Data_sh->args[1], "setenv") == 0)
		func_helper_set_env();
	else if (_strcmp(Data_sh->args[1], "env") == 0)
		func_helper_env();
	else if (_strcmp(Data_sh->args[1], "unset_env") == 0)
		func_helper_unset_env();
	else if (_strcmp(Data_sh->args[1], "help") == 0)
		func_helper();
	else if (_strcmp(Data_sh->args[1], "exit") == 0)
		func_helper_exit();
	else if (_strcmp(Data_sh->args[1], "cd") == 0)
		func_helper_cd();
	else if (_strcmp(Data_sh->args[1], "alias") == 0)
		func_helper_alias();
	else
		write(STDERR_FILENO, Data_sh->args[0],
		      _strlen(Data_sh->args[0]));

	Data_sh->status = 0;
	return (1);
}


/**
 * bring_line - assigns the line var for get_line
 * @lineptr: Buffer that store the user_in str
 * @buffer: str that is been called to line
 * @n: size of line
 * @j: size of buffer
 */
void bring_line(char **lineptr, size_t *n, char *buffer, size_t j)
{

	if (*lineptr == NULL)
	{
		if  (j > BUFFERSIZE)
			*n = j;

		else
			*n = BUFFERSIZE;
		*lineptr = buffer;
	}
	else if (*n < j)
	{
		if (j > BUFFERSIZE)
			*n = j;
		else
			*n = BUFFERSIZE;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}
/**
 * get_line - Read stdin from stream
 * @lineptr: buffer that stores the user_in
 * @n: size of lineptr
 * @stream: stream to read from
 * Return: The number of bytes
 */
ssize_t get_line(char **lineptr, size_t *n, FILE *stream)
{
	int i;
	static ssize_t user_in;
	ssize_t retval;
	char *buffer;
	char t = 'z';

	if (user_in == 0)
		fflush(stream);
	else
		return (-1);
	user_in = 0;

	buffer = malloc(sizeof(char) * BUFFERSIZE);
	if (buffer == 0)
		return (-1);
	while (t != '\n')
	{
		i = read(STDIN_FILENO, &t, 1);
		if (i == -1 || (i == 0 && user_in == 0))
		{
			free(buffer);
			return (-1);
		}
		if (i == 0 && user_in != 0)
		{
			user_in++;
			break;
		}
		if (user_in >= BUFFERSIZE)
			buffer = func_realloc(buffer, user_in, user_in + 1);
		buffer[user_in] = t;
		user_in++;
	}
	buffer[user_in] = '\0';
	bring_line(lineptr, n, buffer, user_in);
	retval = user_in;
	if (i != 0)
		user_in = 0;
	return (retval);
}


/**
 * hundel_ctrd - Handle the crtl + c call in prompt
 * @sig: Signal
 */
void hundel_ctrd(int s)
{
	(void)s;
	write(STDOUT_FILENO, "\n$ ", 3);
}


/**
 * free_data - frees data strcture
 *
 * @Data_sh: data strcture
 * Return: void
 */
void free_data(Data_sl *Data_sh)
{
	unsigned int i;

	for (i = 0; Data_sh->var_environ[i]; i++)
	{
		free(Data_sh->var_environ[i]);
	}

	free(Data_sh->var_environ);
	free(Data_sh->pid);
}

/**
 * set_data - Initialize data strcture
 *
 * @Data_sh: data strcture
 * @av: argument vector
 * Return: void
 */
void set_data(Data_sl *Data_sh, char **av)
{
	unsigned int i;

	Data_sh->av = av;
	Data_sh->user_in = NULL;
	Data_sh->args = NULL;
	Data_sh->status = 0;
	Data_sh->ln_count = 1;

	for (i = 0; environ[i]; i++)
		;

	Data_sh->var_environ = malloc(sizeof(char *) * (i + 1));

	for (i = 0; environ[i]; i++)
	{
		Data_sh->var_environ[i] = _strdup(environ[i]);
	}

	Data_sh->var_environ[i] = NULL;
	Data_sh->pid = func_itoa(getpid());
}

/**
 * read_line - reads the user_in str.
 * @End_of: return value of getline function
 * Return: user_in str
 */
char *read_line(int *End_of)
{
	char *user_in = NULL;
	size_t buffersize = 0;

	*End_of = getline(&user_in, &buffersize, stdin);

	return (user_in);
}


/**
 * check_env - checks if the typed variable is an env variable
 * @h: head of linked list
 * @in: user_in str
 * @data: data strcture
 * Return: void
 */
void check_env(StoreVar **h, char *in, Data_sl *data)
{
	int row, chr, j, lenval;
	char **_envr;

	_envr = data->var_environ;
	for (row = 0; _envr[row]; row++)
	{
		for (j = 1, chr = 0; _envr[row][chr]; chr++)
		{
			if (_envr[row][chr] == '=')
			{
				lenval = _strlen(_envr[row] + chr + 1);
				add_Storevar(h, j, _envr[row] + chr + 1, lenval);
				return;
			}

			if (in[j] == _envr[row][chr])
				j++;
			else
				break;
		}
	}

	for (j = 0; in[j]; j++)
	{
		if (in[j] == ' ' || in[j] == '\t' || in[j] == ';' || in[j] == '\n')
			break;
	}

	add_Storevar(h, j, NULL, 0);
}

/**
 * check_vars - check if the typed variable is $$ or $?
 * @h: head of the linked list
 * @in: user_in str
 * @st: last status of the Shell
 * @data: data strcture
 * Return: void
 */
int check_vars(StoreVar **h, char *in, char *st, Data_sl *data)
{
	int i, lst, lpd;

	lst = _strlen(st);
	lpd = _strlen(data->pid);

	for (i = 0; in[i]; i++)
	{
		if (in[i] == '$')
		{
			if (in[i + 1] == '?')
				add_Storevar(h, 2, st, lst), i++;
			else if (in[i + 1] == '$')
				add_Storevar(h, 2, data->pid, lpd), i++;
			else if (in[i + 1] == '\n')
				add_Storevar(h, 0, NULL, 0);
			else if (in[i + 1] == '\0')
				add_Storevar(h, 0, NULL, 0);
			else if (in[i + 1] == ' ')
				add_Storevar(h, 0, NULL, 0);
			else if (in[i + 1] == '\t')
				add_Storevar(h, 0, NULL, 0);
			else if (in[i + 1] == ';')
				add_Storevar(h, 0, NULL, 0);
			else
				check_env(h, in + i, data);
		}
	}

	return (i);
}

/**
 * replaced_user_in - replaces str into variables
 * @head: head of the linked list
 * @user_in: user_in str
 * @new_user_in: new user_in str (replaced)
 * @nlen: new length
 * Return: replaced str
 */
char *replaced_user_in(StoreVar **head, char *user_in, char *new_user_in, int nlen)
{
	StoreVar *indx;
	int i, j, k;

	indx = *head;
	for (j = i = 0; i < nlen; i++)
	{
		if (user_in[j] == '$')
		{
			if (!(indx->len_var) && !(indx->len_val))
			{
				new_user_in[i] = user_in[j];
				j++;
			}
			else if (indx->len_var && !(indx->len_val))
			{
				for (k = 0; k < indx->len_var; k++)
					j++;
				i--;
			}
			else
			{
				for (k = 0; k < indx->len_val; k++)
				{
					new_user_in[i] = indx->val[k];
					i++;
				}
				j += (indx->len_var);
				i--;
			}
			indx = indx->next;
		}
		else
		{
			new_user_in[i] = user_in[j];
			j++;
		}
	}

	return (new_user_in);
}

/**
 * rep_var - calls functions to replace str into vars
 * @user_in: user_in str
 * @Data_sh: data strcture
 * Return: replaced str
 */
char *rep_var(char *user_in, Data_sl *Data_sh)
{
	StoreVar *head, *indx;
	char *status, *new_user_in;
	int olen, nlen;

	status = func_itoa(Data_sh->status);
	head = NULL;

	olen = check_vars(&head, user_in, status, Data_sh);

	if (head == NULL)
	{
		free(status);
		return (user_in);
	}

	indx = head;
	nlen = 0;

	while (indx != NULL)
	{
		nlen += (indx->len_val - indx->len_var);
		indx = indx->next;
	}

	nlen += olen;

	new_user_in = malloc(sizeof(char) * (nlen + 1));
	new_user_in[nlen] = '\0';

	new_user_in = replaced_user_in(&head, user_in, new_user_in, nlen);

	free(user_in);
	free(status);
	free_StoreVar(&head);

	return (new_user_in);
}


/**
 * cls_comment - clear comments from user_in
 * @in: user_in str
 * Return: user_in
 */
char *cls_comment(char *in)
{
	int i, j;

	j = 0;
	for (i = 0; in[i]; i++)
	{
		if (in[i] == '#')
		{
			if (i == 0)
			{
				free(in);
				return (NULL);
			}

			if (in[i - 1] == ' ' || in[i - 1] == '\t' || in[i - 1] == ';')
				j = i;
		}
	}

	if (j != 0)
	{
		in = func_realloc(in, i, j + 1);
		in[j] = '\0';
	}

	return (in);
}

/**
 * shell_loop - looper
 * @Data_sh: data (av, user_in, args)
 * Return: void.
 */
void shell_loop(Data_sl *Data_sh)
{
	int prompte, End_of;
	char *user_in;

	prompte = 1;
	while (prompte == 1)
	{
		write(STDIN_FILENO, "$ ", 2);
		user_in = read_line(&End_of);
		if (End_of != -1)
		{
			user_in = cls_comment(user_in);
			if (user_in == NULL)
				continue;

			if (check_error(Data_sh, user_in) == 1)
			{
				Data_sh->status = 2;
				free(user_in);
				continue;
			}
			user_in = rep_var(user_in, Data_sh);
			prompte = cmd_spliter(Data_sh, user_in);
			Data_sh->ln_count += 1;
			free(user_in);
		}
		else
		{
			prompte = 0;
			free(user_in);
		}
	}
}


/**
 * swap_char - swaps | and & for non-printed char
 * @user_in: user_in str
 * @bool: swap
 * Return: swapped str
 */
char *swap_char(char *user_in, int bool)
{
	int i;

	if (bool == 0)
	{
		for (i = 0; user_in[i]; i++)
		{
			if (user_in[i] == '|')
			{
				if (user_in[i + 1] != '|')
					user_in[i] = 16;
				else
					i++;
			}

			if (user_in[i] == '&')
			{
				if (user_in[i + 1] != '&')
					user_in[i] = 12;
				else
					i++;
			}
		}
	}
	else
	{
		for (i = 0; user_in[i]; i++)
		{
			user_in[i] = (user_in[i] == 16 ? '|' : user_in[i]);
			user_in[i] = (user_in[i] == 12 ? '&' : user_in[i]);
		}
	}
	return (user_in);
}

/**
 * add_nodes - add separators and cmd lines in the lists
 * @head_s: head separator list
 * @head_l: head cmd lines list
 * @user_in: user_in str
 * Return: void
 */
void add_nodes(sperator_lst **head_s, list_ln **head_l, char *user_in)
{
	int i;
	char *ln;

	user_in = swap_char(user_in, 0);

	for (i = 0; user_in[i]; i++)
	{
		if (user_in[i] == ';')
			add_node(head_s, user_in[i]);

		if (user_in[i] == '|' || user_in[i] == '&')
		{
			add_node(head_s, user_in[i]);
			i++;
		}
	}

	ln = _strtok(user_in, ";|&");
	do {
		ln = swap_char(ln, 1);
		AddLineNode(head_l, ln);
		ln = _strtok(NULL, ";|&");
	} while (ln != NULL);

}

/**
 * go_next - go to the next cmd
 * @list_s: separator list
 * @list_l: cmd line list
 * @Data_sh: strcture
 * Return: void
 */
void go_next(sperator_lst **list_s, list_ln **list_l, Data_sl *Data_sh)
{
	int check_sep;
	sperator_lst *ls_s;
	list_ln *ls_l;

	check_sep = 1;
	ls_s = *list_s;
	ls_l = *list_l;

	while (ls_s != NULL && check_sep)
	{
		if (Data_sh->status == 0)
		{
			if (ls_s->separator == '&' || ls_s->separator == ';')
				check_sep = 0;
			if (ls_s->separator == '|')
				ls_l = ls_l->next, ls_s = ls_s->next;
		}
		else
		{
			if (ls_s->separator == '|' || ls_s->separator == ';')
				check_sep = 0;
			if (ls_s->separator == '&')
				ls_l = ls_l->next, ls_s = ls_s->next;
		}
		if (ls_s != NULL && !check_sep)
			ls_s = ls_s->next;
	}

	*list_s = ls_s;
	*list_l = ls_l;
}

/**
 * cmd_spliter - splits cmd lines according to sep
 * @Data_sh: strcture
 * @user_in: user_in str
 * Return: 0 to exit
 */
int cmd_spliter(Data_sl *Data_sh, char *user_in)
{

	sperator_lst *head_s, *list_s;
	list_ln *head_l, *list_l;
	int lp;

	head_s = NULL;
	head_l = NULL;

	add_nodes(&head_s, &head_l, user_in);

	list_s = head_s;
	list_l = head_l;

	while (list_l != NULL)
	{
		Data_sh->user_in = list_l->line;
		Data_sh->args = line_spliter(Data_sh->user_in);
		lp = exec_line(Data_sh);
		free(Data_sh->args);

		if (lp == 0)
			break;

		go_next(&list_s, &list_l, Data_sh);

		if (list_l != NULL)
			list_l = list_l->next;
	}

	free_sperator_lst(&head_s);
	free_list_ln(&head_l);

	if (lp == 0)
		return (0);
	return (1);
}

/**
 * line_spliter - tokenizes the user_in str
 *
 * @user_in: user_in str.
 * Return: str.
 */
char **line_spliter(char *user_in)
{
	size_t t_size;
	size_t i;
	char **tokens;
	char *token;

	t_size = TOK_BUFFERSIZE;
	tokens = malloc(sizeof(char *) * (t_size));
	if (tokens == NULL)
	{
		write(STDERR_FILENO, ": allocation error\n", 18);
		exit(EXIT_FAILURE);
	}

	token = _strtok(user_in, TOK_DELIM);
	tokens[0] = token;

	for (i = 1; token != NULL; i++)
	{
		if (i == t_size)
		{
			t_size += TOK_BUFFERSIZE;
			tokens = func_reallocdp(tokens, i, sizeof(char *) * t_size);
			if (tokens == NULL)
			{
				write(STDERR_FILENO, ": allocation error\n", 18);
				exit(EXIT_FAILURE);
			}
		}
		token = _strtok(NULL, TOK_DELIM);
		tokens[i] = token;
	}

	return (tokens);
}


/**
 * main - Entry point
 * @ac: argument count
 * @av: argument vector
 * Return: 0 on success.
 */
int main(int ac, char **av)
{
	Data_sl Data_sh;
	(void) ac;

	signal(SIGINT, hundel_ctrd);
	set_data(&Data_sh, av);
	shell_loop(&Data_sh);
	free_data(&Data_sh);
	if (Data_sh.status < 0)
		return (255);
	return (Data_sh.status);
}
