#include "env_handler.h"

char	**_env_handler_create_envtable_origin(t_env_handler *env_handler)
{
	char	**new_envlist;
	char	**new_envlist_ptr;
	char	**envlist_ptr;
	size_t	count;

	count = 0;
	envlist_ptr = env_handler->envp_origin;
	while (*envlist_ptr)
	{
		count++;
		envlist_ptr++;
	}
	new_envlist = malloc((count + 1) * sizeof(char *));
	if (!new_envlist)
		return (NULL);
	envlist_ptr = env_handler->envp_origin;
	new_envlist_ptr = new_envlist;
	while (*envlist_ptr)
		*new_envlist_ptr++ = *envlist_ptr++;
	*new_envlist_ptr++ = NULL;
	return (new_envlist);
}

char	**env_handler_create_envtable(t_env_handler *env_handler)
{
	char			**new_envlist;
	char			**new_envlist_ptr;
	size_t			h;
	t_env_node_i	node;

	if (!env_handler->hash_table)
		return (_env_handler_create_envtable_origin(env_handler));
	new_envlist = malloc(env_handler->array.used * sizeof(char *));
	if (!new_envlist)
		return (NULL);
	new_envlist_ptr = new_envlist;
	h = 0;
	while (h < env_handler->hash_table_size)
	{
		node = env_handler->hash_table[h];
		while (node)
		{
			*new_envlist_ptr++ = env_handler->nodes[node].var_str;
			node = env_handler->nodes[node].next_node;
		}
		h++;
	}
	return (*new_envlist_ptr = 0, new_envlist);
}

bool	env_handler_delete_env(t_env_handler *env_handler, char *env_name)
{
	char			*name_end;
	unsigned int	hash;
	unsigned int	str_size;
	t_env_node_i	node;

	str_size = 0;
	if (!env_handler->hash_table)
		if (!_env_handler_init_modifiable(env_handler))
			return (false);
	str_size += _scan_env_str(env_name, &name_end, &hash) + 1;
	hash %= env_handler->hash_table_size;
	node = _env_handler_find_in_list(env_handler,
			env_handler->hash_table[hash], env_name, name_end - env_name);
	if (!node)
		return (NULL);
	env_handler->hash_table[hash]
		= _env_handler_list_unlink_node(env_handler,
			env_handler->hash_table[hash], node);
	return (NULL);
}

char	*env_handler_get_env_value(t_env_handler *env_handler, char *env_name)
{
	char			*result_str;
	char			*name_end;
	unsigned int	hash;
	unsigned int	str_size;
	t_env_node_i	node;

	str_size = 0;
	if (env_handler->hash_table)
	{
		str_size += _scan_env_str(env_name, &name_end, &hash) + 1;
		hash %= env_handler->hash_table_size;
		node = _env_handler_find_in_list(env_handler,
				env_handler->hash_table[hash], env_name, name_end - env_name);
		if (!node)
			return (NULL);
		result_str = env_handler->nodes[node].var_str + (name_end - env_name);
		result_str += (*result_str == '=');
		return (result_str);
	}
	return (getenv(env_name));
}

// Returns if the has a value or not (ie export ENV with no assign) 
// Returns false if ENV doesnot exist, so test before with
// get_env_value() return NULL
// Imported envs (original table) are not NULL
bool	env_handler_get_env_is_null(t_env_handler *env_handler, char *env_name)
{
	char			*name_end;
	unsigned int	hash;
	unsigned int	str_size;
	t_env_node_i	node;

	str_size = 0;
	if (env_handler->hash_table)
	{
		str_size += _scan_env_str(env_name, &name_end, &hash) + 1;
		hash %= env_handler->hash_table_size;
		node = _env_handler_find_in_list(env_handler,
				env_handler->hash_table[hash], env_name, name_end - env_name);
		if (!node)
			return (false);
		return (!env_handler->nodes[node].var_str[name_end - env_name]);
	}
	return (false);
}
