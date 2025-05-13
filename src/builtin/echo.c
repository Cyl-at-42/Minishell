#include "builtin.h"

static bool	_builtin_echo_parse_optchain(char *res_opts, char *optchain)
{
	int		pos;
	bool	flag_n_here;

	flag_n_here = false;
	if (*optchain != '-')
		return (false);
	pos = 1;
	while (optchain[pos])
	{
		if (optchain[pos] == 'n')
			flag_n_here = true;
		else
			return (false);
		pos++;
	}
	if (pos == 1)
		return (false);
	if (flag_n_here)
		res_opts[0] = 'n';
	if (res_opts[0])
		res_opts[1] = 0;
	return (true);
}

int	builtin_echo(t_shell *shell)
{
	int		i;
	int		argc;
	char	**argv;
	char	opts[4];

	argc = shell->argv_array.used - 1;
	argv = shell->argv_array.data;
	i = 1;
	opts[0] = 0;
	if (argc > 1)
	{
		while (argv[i] && _builtin_echo_parse_optchain(opts, argv[i]))
			i++;
		while (i < argc)
		{
			write(1, argv[i], ft_strlen(argv[i]));
			i++;
			if (i < argc)
				write(1, " ", 1);
		}
	}
	if (!opts[0])
		write(1, "\n", 1);
	return (0);
}
