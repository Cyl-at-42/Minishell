#include "minishell.h"

void	builtins_tinystr_copy(unsigned long *dest, char *source)
{
	char	*dest2;
	int		i;

	dest2 = (char *)dest;
	*((unsigned long *)dest) = 0;
	i = 0;
	while (*source && (i++ < 8))
		*dest2++ = *source++;
}

void	builtins_init_tinystrs(unsigned long *builtins_tinystrs)
{
	builtins_tinystr_copy(&builtins_tinystrs[0], "cd");
	builtins_tinystr_copy(&builtins_tinystrs[1], "env");
	builtins_tinystr_copy(&builtins_tinystrs[2], "pwd");
	builtins_tinystr_copy(&builtins_tinystrs[3], "echo");
	builtins_tinystr_copy(&builtins_tinystrs[4], "exit");
	builtins_tinystr_copy(&builtins_tinystrs[5], "unset");
	builtins_tinystr_copy(&builtins_tinystrs[6], "export");
}

void	*str_is_builtin(char *str)
{
	static unsigned long	builtins[7];
	unsigned long			tinystr;

	if (!builtins[0])
		builtins_init_tinystrs(builtins);
	builtins_tinystr_copy(&tinystr, str);
	if (tinystr == builtins[0])
		return (builtin_cd);
	if (tinystr == builtins[1])
		return (builtin_env);
	if (tinystr == builtins[2])
		return (builtin_pwd);
	if (tinystr == builtins[3])
		return (builtin_echo);
	if (tinystr == builtins[4])
		return (builtin_exit);
	if (tinystr == builtins[5])
		return (builtin_unset);
	if (tinystr == builtins[6])
		return (builtin_export);
	return (NULL);
}
