#include "env_handler.h"

bool	_env_handler_init_nodes(t_env_handler *env_handler)
{
	t_env_node_i	new_element;
	char			**envp_ptr;
	char			*name_end;
	unsigned int	hash;

	if (!xarray_new_element(&env_handler->array, &new_element))
		return (false);
	env_handler->nodes[0].var_str = NULL;
	env_handler->nodes[0].cell_size = 0;
	env_handler->nodes[0].next_node = 0;
	envp_ptr = env_handler->envp_origin;
	while (*envp_ptr)
	{
		if (!xarray_new_element(&env_handler->array, &new_element))
			return (false);
		env_handler->nodes[new_element].var_str = *envp_ptr;
		env_handler->nodes[new_element].cell_size = 0;
		_scan_env_str(*envp_ptr++, &name_end, &hash);
		hash %= env_handler->hash_table_size;
		env_handler->nodes[new_element].next_node
			= env_handler->hash_table[hash];
		env_handler->hash_table[hash] = new_element;
	}
	return (true);
}

// retourne la tete de liste, eventuellement, modifiee 
t_env_node_i	_env_handler_list_unlink_node(t_env_handler *env_handler,
	t_env_node_i list, t_env_node_i node)
{
	t_env_node_i	prev_node;
	t_env_node_i	return_node;

	return_node = list;
	prev_node = _env_handler_prevnode(env_handler, list, node);
	if (prev_node)
		env_handler->nodes[prev_node].next_node
			= env_handler->nodes[node].next_node;
	else
		return_node = env_handler->nodes[node].next_node;
	prev_node = env_handler->nodes[0].next_node;
	env_handler->nodes[0].next_node = node;
	env_handler->nodes[node].next_node = prev_node;
	return (return_node);
}

t_env_node_i	_env_handler_prevnode(t_env_handler *env_handler,
	t_env_node_i list, t_env_node_i node)
{
	t_env_node_i	next_node;

	if (node == list)
		return (0);
	while (list)
	{
		next_node = env_handler->nodes[list].next_node;
		if (next_node == node)
			return (list);
		list = next_node;
	}
	return (0);
}

t_env_node_i	_env_handler_find_in_list(t_env_handler *env_handler,
	t_env_node_i node, char *env_str, size_t envname_len)
{
	char	*var_str;
	char	chr_after_name;

	if (!node)
		return (0);
	while (node)
	{
		var_str = env_handler->nodes[node].var_str;
		if (ft_strncmp(env_str, var_str, envname_len) == 0)
		{
			chr_after_name = env_handler->nodes[node].var_str[envname_len];
			if ((!chr_after_name) || (chr_after_name == '='))
				return (node);
		}
		node = env_handler->nodes[node].next_node;
	}
	return (0);
}
