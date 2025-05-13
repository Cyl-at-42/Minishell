#include "builtin.h"

static void	_export_sort_env(char **env_table, size_t env_size)
{
	size_t	i;
	char	*temp;

	if (env_size <= 1)
		return ;
	while (env_size-- != 0)
	{
		i = 0;
		while (i < env_size)
		{
			if (ft_strcmp(env_table[i], env_table[i + 1]) > 0)
			{
				temp = env_table[i];
				env_table[i] = env_table[i + 1];
				env_table[i + 1] = temp;
			}
			i++;
		}
	}
}

static size_t	_export_get_env_size(char	**env_table)
{
	size_t	size;

	size = 0;
	while (*env_table)
	{
		size++;
		env_table++;
	}
	return (size);
}

static char	*_get_equal_pos(char *var)
{
	while (*var && *var != '=')
		var++;
	return (var);
}

void	print_env(char *env_table_ptr)
{
	char	*equal_pos;
	size_t	pos_diff;

	if (ft_strlen(env_table_ptr) >= 2
		&& *env_table_ptr == '_' && *(env_table_ptr + 1) == '=')
		return ;
	equal_pos = _get_equal_pos(env_table_ptr);
	pos_diff = equal_pos - env_table_ptr;
	write(1, "declare -x ", 11);
	if (strchr(env_table_ptr, '='))
	{
		write(1, env_table_ptr, pos_diff);
		write(1, "=\"", 2);
		write(1, env_table_ptr + pos_diff + 1,
			ft_strlen(env_table_ptr + pos_diff + 1));
		write(1, "\"", 1);
	}
	else
		write(1, env_table_ptr, ft_strlen(env_table_ptr));
	write(1, "\n", 1);
	env_table_ptr++;
}

bool	export_show_env(t_env_handler *env_handler)
{
	char	**env_table;
	char	**env_table_ptr;

	env_table = env_handler_create_envtable(env_handler);
	if (!env_table)
		return (write_error_strs(1, "export: Cannot create table\n")
			, EXIT_FAILURE);
	_export_sort_env(env_table, _export_get_env_size(env_table));
	env_table_ptr = env_table;
	while (*env_table_ptr)
	{
		print_env(*env_table_ptr);
		env_table_ptr++;
	}
	free(env_table);
	return (EXIT_SUCCESS);
}
