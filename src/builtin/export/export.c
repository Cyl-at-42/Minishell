#include "builtin.h"

int	builtin_export(t_shell *shell)
{
	int		argc;
	char	**argv;
	bool	return_value;

	argc = shell->argv_array.used - 1;
	argv = shell->argv_array.data;
	if (argc < 2)
	{
		return (export_show_env(shell->env_handler));
	}
	else
	{
		return_value = EXIT_SUCCESS;
		while (argc > 1)
		{
			return_value
				|= export_assign_variable(shell->env_handler, argv[argc - 1]);
			argc--;
		}
		return (return_value);
	}
}
