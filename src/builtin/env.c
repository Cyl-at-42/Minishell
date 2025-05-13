#include "builtin.h"

int	builtin_env(t_shell *shell)
{
	char	**env_table;
	char	**env_table_ptr;

	env_table = env_handler_create_envtable(shell->env_handler);
	if (!env_table)
		return (write_error_strs(1, "export: Cannot create table\n")
			, EXIT_FAILURE);
	env_table_ptr = env_table;
	while (*env_table_ptr)
	{
		if (!ft_strrchr(*env_table_ptr, '='))
		{
			env_table_ptr++;
			continue ;
		}
		write(STDOUT_FILENO, *env_table_ptr, ft_strlen(*env_table_ptr));
		write(STDOUT_FILENO, "\n", 1);
		env_table_ptr++;
	}
	free(env_table);
	return (EXIT_SUCCESS);
}
