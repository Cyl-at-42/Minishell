#include "minishell.h"

bool	shell_exec_word(t_shell *shell, t_ast_node_i node)
{
	char		*dest;
	t_xstring	expd_xstring;

	if (!init_xstring(&expd_xstring, 64))
		return (false);
	if (!shell_extend_string(shell, &expd_xstring, shell->ast->nodes[node].str))
		return (destroy_xstring(&expd_xstring), false);
	dest = xbuffer_alloc(shell->word_buffer,
			estr_str_to_estr(NULL, (char *)expd_xstring.data));
	if (!dest)
		return (destroy_xstring(&expd_xstring), false);
	estr_str_to_estr(dest, expd_xstring.data);
	destroy_xstring(&expd_xstring);
	return (true);
}

bool	shell_exec_dquoted(t_shell *shell, t_ast_node_i node)
{
	char		*dest;
	t_xstring	expd_xstring;

	if (!init_xstring(&expd_xstring, 64))
		return (false);
	if (!shell_extend_string(shell, &expd_xstring, shell->ast->nodes[node].str))
		return (destroy_xstring(&expd_xstring), false);
	dest = xbuffer_alloc(shell->word_buffer, expd_xstring.len << 1);
	if (!dest)
		return (destroy_xstring(&expd_xstring), false);
	escstr_esc_add_substr(dest, expd_xstring.data,
		(char *)expd_xstring.data + expd_xstring.len);
	destroy_xstring(&expd_xstring);
	shell->word_hasquote = true;
	return (true);
}

bool	shell_exec_squoted(t_shell *shell, t_ast_node_i node)
{
	char	*dest;
	char	*str_start;
	char	*str_end;
	size_t	len;

	str_start = shell->ast->nodes[node].str;
	len = ft_strlen(str_start);
	str_end = str_start + len;
	dest = xbuffer_alloc(shell->word_buffer, len << 1);
	if (!dest)
		return (false);
	escstr_esc_add_substr(dest, str_start, str_end);
	shell->word_hasquote = true;
	return (true);
}

bool	shell_process_word_buffer(t_shell *shell)
{
	char			*all_word;
	char			**all_word_array;
	size_t			size;
	unsigned int	args_added;
	unsigned int	prev_arg_count;

	size = xbuffer_size(shell->word_buffer);
	all_word = xbuffer_alloc(shell->generated_args_buffer, size + 1);
	if (!all_word)
		return (false);
	xbuffer_copy_data(shell->word_buffer, all_word);
	all_word[size] = 0;
	all_word_array = escstr_replacesplit(all_word);
	if (!all_word_array)
		return (false);
	prev_arg_count = shell_get_args_count(shell);
	if (!shell_word_array_to_args(shell, all_word_array, &args_added))
		return (free(all_word_array), false);
	if (!args_added)
		shell_string_to_args(shell, all_word);
	if (!shell->word_hasquote && (shell_get_args_count(shell)
			== (prev_arg_count + 1)))
		shell_arg_cancel_empty_string(shell);
	delete_xbuffer(shell->word_buffer);
	return (free(all_word_array), shell->word_buffer = NULL, true);
}

bool	shell_exec_word_elements(t_shell *shell, t_ast_node_i node)
{
	unsigned int	node_type;

	if (shell->word_buffer)
	{
		node_type = shell->ast->nodes[node].type;
		if (node_type == 'w')
			return (shell_exec_word(shell, node));
		else if (node_type == PARSER_NODE_TYPE_WORD_ELTS)
			return (shell_exec_children(shell, node));
		else if (node_type == FINISHED_DOUBLE_QUOTE)
			return (shell_exec_dquoted(shell, node));
		else if (node_type == FINISHED_SINGLE_QUOTE)
			return (shell_exec_squoted(shell, node));
	}
	else
	{
		shell->word_buffer = create_xbuffer(32);
		if (!shell->word_buffer)
			return (false);
		if (!shell_exec_word_elements(shell, node))
			return (false);
		return (shell_process_word_buffer(shell));
	}
	return (true);
}
