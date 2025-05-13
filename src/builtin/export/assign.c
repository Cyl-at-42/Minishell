#include "builtin.h"

static void	not_a_valid_identifier(char *cmd)
{
	if (cmd && cmd[0])
	{
		write_error_strs(4, APP_NAME, ": export: `", cmd,
			"': not a valid identifier\n");
	}
	else
	{
		write_error_strs(3, APP_NAME, ": export: `",
			"': not a valid identifier\n");
	}
}

bool	export_assign_variable(t_env_handler *env_handler, char *arg)
{
	size_t	index;

	index = 0;
	if (!ft_isalpha(*arg) && arg[0] != '_')
		return (not_a_valid_identifier(arg), EXIT_FAILURE);
	while (arg[index] != '=' && arg[index])
	{
		if (!ft_isalpha(arg[index]) && !ft_isdigit(arg[index])
			&& arg[index] != '_')
			return (not_a_valid_identifier(arg), EXIT_FAILURE);
		index++;
	}
	if (index == 0)
		return (not_a_valid_identifier(arg), EXIT_FAILURE);
	if (!ft_strrchr(arg, '='))
	{
		if (!env_handler_get_env_value(env_handler, arg))
			return (env_handler_set(env_handler, arg));
		return (EXIT_SUCCESS);
	}
	else
		return (!env_handler_set(env_handler, arg));
}
