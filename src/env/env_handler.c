#include "env_handler.h"

bool	init_env_handler(t_env_handler *env_handler,
	unsigned int hash_table_size, char **envp)
{
	ft_memset(env_handler, 0, sizeof(t_env_handler));
	env_handler->envp_origin = envp;
	env_handler->hash_table_size = hash_table_size;
	return (true);
}

void	destroy_env_handler(t_env_handler *env_handler)
{
	destroy_xarray(&env_handler->array);
	delete_xbuffer(env_handler->buffer);
	free(env_handler->hash_table);
}

unsigned int	_scan_env_str(char *env_str, char **name_end,
	unsigned int *hash)
{
	char			*env_str_save;

	*hash = 0;
	env_str_save = env_str;
	while (*env_str)
	{
		if (!*env_str || *env_str == '=')
			break ;
		*hash += *env_str * 65599;
		env_str++;
	}
	*name_end = env_str;
	if (*env_str)
	{
		while (*env_str++)
			;
		env_str--;
	}
	return (env_str - env_str_save);
}

bool	_env_handler_init_modifiable(t_env_handler *env_handler)
{
	char	**envp_ptr;
	size_t	size;

	envp_ptr = env_handler->envp_origin;
	while (*envp_ptr++)
		;
	size = env_handler->hash_table_size * sizeof(t_env_node_i);
	env_handler->hash_table = malloc(size);
	env_handler->buffer = create_xbuffer(0);
	if (!init_xarray(&env_handler->array, 128, sizeof(t_env_node))
		|| !env_handler->hash_table || !env_handler->buffer)
		return (destroy_env_handler(env_handler), false);
	ft_memset(env_handler->hash_table, 0, size);
	env_handler->nodes = env_handler->array.data;
	return (_env_handler_init_nodes(env_handler));
}
