#include "minishell.h"

bool	shell_word_array_to_args(t_shell *shell, char **word_array_ptr,
	unsigned int *args_added)
{
	t_xarray		*target_array;
	unsigned int	new_element;

	target_array = &shell->argv_array;
	if (shell->redir_args_array.data)
		target_array = &shell->redir_args_array;
	*args_added = target_array->used;
	while (*word_array_ptr)
	{
		if (wpm_is_wildcard_pattern(*word_array_ptr))
		{
			if (!shell_wildcard_populate_args(shell, target_array,
					*word_array_ptr))
				return (false);
		}
		else
		{
			if (!xarray_new_element(target_array, &new_element))
				return (false);
			escstr_unescape_all(*word_array_ptr);
			((char **)target_array->data)[new_element] = *word_array_ptr;
		}
		word_array_ptr++;
	}
	return (*args_added = target_array->used - *args_added, true);
}

bool	shell_string_to_args(t_shell *shell, char *str)
{
	t_xarray		*target_array;
	unsigned int	new_element;

	target_array = &shell->argv_array;
	if (shell->redir_args_array.data)
		target_array = &shell->redir_args_array;
	if (!xarray_new_element(target_array, &new_element))
		return (false);
	escstr_unescape_all(str);
	((char **)target_array->data)[new_element] = str;
	return (true);
}

unsigned int	shell_get_args_count(t_shell *shell)
{
	t_xarray		*target_array;

	target_array = &shell->argv_array;
	if (shell->redir_args_array.data)
		target_array = &shell->redir_args_array;
	return (target_array->used);
}

// Decrease table size if last item is an empty str
void	shell_arg_cancel_empty_string(t_shell *shell)
{
	t_xarray		*target_array;
	char			**strtable;

	target_array = &shell->argv_array;
	if (shell->redir_args_array.data)
		target_array = &shell->redir_args_array;
	strtable = (char **)target_array->data;
	if (*strtable[target_array->used - 1])
		return ;
	target_array->used--;
	return ;
}
