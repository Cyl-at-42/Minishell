#include "env_handler.h"
#include "xstring.h"

bool	_env_handler_set_modify(t_env_handler *env_handler,
	t_env_node_i node, char *env_str, unsigned int str_size)
{
	char	*str;

	if (env_handler->nodes[node].cell_size)
	{
		if (str_size < env_handler->nodes[node].cell_size)
			ft_memcpy(env_handler->nodes[node].var_str, env_str, str_size);
		else
		{
			str = xbuffer_alloc(env_handler->buffer, str_size);
			if (!str)
				return (false);
			ft_memcpy(str, env_str, str_size);
			env_handler->nodes[node].var_str = str;
			env_handler->nodes[node].cell_size = str_size;
		}
		return (true);
	}
	str = xbuffer_alloc(env_handler->buffer, str_size);
	if (!str)
		return (false);
	ft_memcpy(str, env_str, str_size);
	env_handler->nodes[node].var_str = str;
	env_handler->nodes[node].cell_size = str_size;
	return (true);
}

bool	_env_handler_set_add(t_env_handler *env_handler, char *env_str,
	unsigned int str_size, unsigned int hash)
{
	char			*str;
	t_env_node_i	node;

	if (!xarray_new_element(&env_handler->array, &node))
		return (false);
	env_handler->nodes = env_handler->array.data;
	str = xbuffer_alloc(env_handler->buffer, str_size);
	if (!str)
		return (false);
	ft_memcpy(str, env_str, str_size);
	env_handler->nodes[node].var_str = str;
	env_handler->nodes[node].cell_size = str_size;
	env_handler->nodes[node].next_node = env_handler->hash_table[hash];
	env_handler->hash_table[hash] = node;
	return (true);
}

bool	env_handler_set(t_env_handler *env_handler, char *env_str)
{
	char			*name_end;
	unsigned int	hash;
	unsigned int	str_size;
	t_env_node_i	node;

	if (!env_handler->hash_table)
		if (!_env_handler_init_modifiable(env_handler))
			return (false);
	str_size = _scan_env_str(env_str, &name_end, &hash) + 1;
	hash %= env_handler->hash_table_size;
	node = _env_handler_find_in_list(env_handler,
			env_handler->hash_table[hash], env_str, name_end - env_str);
	if (node)
	{
		if (!_env_handler_set_modify(env_handler, node, env_str, str_size))
			return (false);
	}
	else
	{
		if (!_env_handler_set_add(env_handler, env_str, str_size, hash))
			return (false);
	}
	return (true);
}

bool	env_handler_setval(
	t_env_handler *env_handler, char *env_name, char *val)
{
	t_xstring	env_string;

	if (!val)
		return (env_handler_set(env_handler, env_name));
	if (!init_xstring(&env_string, 32))
		return (false);
	if (!xstring_add_string(&env_string, env_name))
		return (destroy_xstring(&env_string), false);
	if (!xstring_add_string(&env_string, "="))
		return (destroy_xstring(&env_string), false);
	if (!xstring_add_string(&env_string, val))
		return (destroy_xstring(&env_string), false);
	env_handler_set(env_handler, (char *)env_string.data);
	destroy_xstring(&env_string);
	return (true);
}
