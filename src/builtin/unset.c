#include "builtin.h"

int	builtin_unset(t_shell *shell)
{
	int		argc;
	char	**argv;

	argc = shell->argv_array.used - 1;
	argv = shell->argv_array.data;
	while (argc > 1)
	{
		env_handler_delete_env(shell->env_handler, argv[argc - 1]);
		argc--;
	}
	return (0);
}
