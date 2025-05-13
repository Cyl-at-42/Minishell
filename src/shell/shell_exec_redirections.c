#include "minishell.h"

static bool	_shell_redir_is_arg_valid(t_shell *shell, char *tokenword)
{
	if (shell->redir_args_array.used == 2)
		return (true);
	write_error_strs(4, APP_NAME, ": ", tokenword,
		": ambiguous redirect\n");
	return (false);
}

static bool	_shell_exec_redirs_exec(t_shell *shell, unsigned int node_type)
{
	char	*filename;
	bool	r;
	bool	print_error;

	print_error = (shell->current_exec->return_code == 0);
	filename = ((char **)shell->redir_args_array.data)[0];
	r = false;
	if ((node_type == '<') && !shell->current_exec->return_code)
		r = exec_redir_infile(shell->current_exec, filename);
	else if ((node_type == '>') && !shell->current_exec->return_code)
		r = exec_redir_outfile(shell->current_exec, filename);
	else if ((node_type == REDIR_APPEND) && !shell->current_exec->return_code)
		r = exec_redir_appendfile(shell->current_exec, filename);
	if (r)
		return (true);
	if (print_error)
		write_error_strs(5, APP_NAME, ": ", filename, ":", "");
	return (false);
}

bool	shell_exec_redirs(t_shell *shell, t_ast_node_i node)
{
	unsigned int	new_element;
	unsigned int	node_type;

	node_type = shell->ast->nodes[node].type;
	if ((node_type == REDIR_HEREDOC) && !shell->current_exec->return_code)
		return (exec_redir_infile(shell->current_exec,
				shell->ast->nodes[node].str), true);
	if (!init_xarray(&shell->redir_args_array, 0, sizeof(char *)))
		return (false);
	if (!shell_exec_subtree(shell, shell->ast->nodes[node].left_node))
		return (false);
	if (!xarray_new_element(&shell->redir_args_array, &new_element))
		return (false);
	((char **)shell->redir_args_array.data)[new_element] = NULL;
	if (!_shell_redir_is_arg_valid(shell, shell->ast->nodes[node].str))
		return (destroy_xarray(&shell->redir_args_array),
			shell->current_exec->return_code = 1, true);
	_shell_exec_redirs_exec(shell, node_type);
	destroy_xarray(&shell->redir_args_array);
	return (true);
}

bool	shell_redirect_pipes(t_shell *shell, t_exec *exec)
{
	t_exec	*next_exec;

	next_exec = exec + 1;
	if (!shell->pipeline_exec_index)
	{
		if (dup2(next_exec->pipe_fds[PIPE_WRITE_END_FD], STDOUT_FILENO) == -1)
			return (write_error_strs(2, "dup2:", ""), false);
	}
	else if (shell->pipeline_exec_index == shell->pipeline_size - 1)
	{
		if (dup2(exec->pipe_fds[PIPE_READ_END_FD], STDIN_FILENO) == -1)
			return (write_error_strs(2, "dup2:", ""), false);
	}
	else
	{
		if (dup2(exec->pipe_fds[PIPE_READ_END_FD], STDIN_FILENO) == -1)
			return (write_error_strs(2, "dup2:", ""), false);
		if (dup2(next_exec->pipe_fds[PIPE_WRITE_END_FD], STDOUT_FILENO) == -1)
			return (write_error_strs(2, "dup2:", ""), false);
	}
	return (true);
}
