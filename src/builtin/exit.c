#include "builtin.h"

bool	check_sign(char *str, size_t *index)
{
	*index = 0;
	while (str[*index] == '+' || str[*index] == '-')
		(*index)++;
	if (*index > 1)
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}

static bool	is_wrong_numeric(char *str)
{
	size_t	index;
	size_t	sign_index;
	size_t	num_nb;

	if (check_sign(str, &sign_index))
		return (true);
	num_nb = 0;
	index = sign_index;
	while (ft_isdigit(str[index]))
	{
		num_nb++;
		index++;
	}
	if (str[index] != '\0' || num_nb == 0
		|| (str[0] == '-'
			&& ft_atoul(str) > (unsigned long)__LONG_MAX__ + 1)
		|| (str[0] != '-'
			&& ft_atoul(str) > __LONG_MAX__))
		return (true);
	while (str[sign_index] == '0')
		sign_index++;
	if (ft_strlen(&str[sign_index]) > 19)
		return (true);
	return (false);
}

static int	arg_exit(t_shell *shell, int argc, char **argv)
{
	if (is_wrong_numeric(argv[1]))
	{
		write_error_strs(4, APP_NAME, ": exit: ", argv[1],
			": numeric argument required\n");
		return (shell->exit = 1, 2);
	}
	if (argc > 2)
	{
		write_error_strs(2, APP_NAME,
			": exit: too many arguments\n");
		return (EXIT_FAILURE);
	}
	shell->exit = 1;
	return (ft_atol(argv[1]) % 256);
}

int	builtin_exit(t_shell *shell)
{
	unsigned int	argc;
	char			**argv;

	argc = shell->argv_array.used - 1;
	argv = shell->argv_array.data;
	if (shell->exec.pid == shell->global_exec.pid && isatty(0))
		write_error_strs(1, "exit\n");
	if (argc == 1)
		return (shell->exit = 1, shell->global_exec.return_code);
	else
		return (arg_exit(shell, argc, argv));
}
