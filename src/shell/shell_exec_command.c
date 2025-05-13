#include "minishell.h"

static bool	_shell_exec_command_exec(t_shell *shell, char **argv)
{
	int		(*builtin_ftn)(t_shell *shell);
	bool	r;

	shell->current_exec->return_code = 1;
	if (!*argv[0])
		return (write_error_strs(1, ": command not found\n"),
			shell->current_exec->return_code = 127, true);
	if (!shell_save_stdio(shell))
		return (false);
	builtin_ftn = str_is_builtin(argv[0]);
	if (builtin_ftn)
		r = shell_execute_builtin_command(shell, builtin_ftn);
	else
		r = shell_execute_ext_command(shell, argv);
	if (!shell_restore_stdio(shell))
		return (false);
	return (r);
}

static bool	_shell_exec_command_retfalse(t_shell *shell)
{
	if (!shell->current_exec->return_code)
		shell->current_exec->return_code = 1;
	if (!shell->pipeline_execs)
		shell->global_exec.return_code = shell->current_exec->return_code;
	return (false);
}

bool	shell_exec_command(t_shell *shell, t_ast_node_i node)
{
	unsigned int	new_element;
	bool			r;

	r = true;
	if (!shell->pipeline_execs)
		memset(&shell->exec, 0, sizeof(t_exec));
	shell->generated_args_buffer = create_xbuffer(0);
	if (!shell->generated_args_buffer
		|| !init_xarray(&shell->argv_array, 16, sizeof(char *))
		|| !shell_exec_subtree(shell, shell->ast->nodes[node].left_node))
		return (_shell_exec_command_retfalse(shell));
	if (!shell->current_exec->return_code)
	{
		if (!xarray_new_element(&shell->argv_array, &new_element))
			return (_shell_exec_command_retfalse(shell));
		((char **)shell->argv_array.data)[new_element] = NULL;
		if (shell->argv_array.used != 1)
			r = _shell_exec_command_exec(shell,
					(char **)shell->argv_array.data);
	}
	if (!shell->pipeline_execs)
		shell->global_exec.return_code = shell->current_exec->return_code;
	return (delete_xbuffer(shell->generated_args_buffer),
		shell->generated_args_buffer = NULL,
		destroy_xarray(&shell->argv_array), r);
}

static void	_shell_execute_builtin_command_child(t_shell *shell,
	int (*builtin_ftn)(t_shell *))
{
	shell->global_exec.pid = -1;
	if (!exec_redirect_files(&shell->global_exec,
			shell->pipeline_exec_index == 0,
			shell->pipeline_exec_index == shell->pipeline_size - 1))
		return ;
	exec_close_redirect_files(&shell->global_exec);
	if (!shell_redirect_pipes(shell, shell->current_exec))
		return ;
	if (!exec_redirect_files(shell->current_exec, true, true))
		return ;
	shell_close_pipes_and_files(shell);
	exec_close_redirect_files(shell->current_exec);
	shell_close_saved_stdio(shell);
	shell->global_exec.return_code = builtin_ftn(shell);
}

bool	shell_execute_builtin_command(t_shell *shell,
	int (*builtin_ftn)(t_shell *))
{
	pid_t	new_pid;

	if (shell->pipeline_execs)
	{
		new_pid = fork();
		if (new_pid == -1)
			return (write_error_strs(2, "fork:", ""), false);
		if (new_pid == 0)
			return (_shell_execute_builtin_command_child(shell, builtin_ftn),
				false);
		shell->current_exec->pid = new_pid;
	}
	else
	{
		if (!exec_redirect_files(&shell->global_exec, true, true))
			return (false);
		exec_close_redirect_files(&shell->global_exec);
		if (!exec_redirect_files(shell->current_exec, true, true))
			return (false);
		exec_close_redirect_files(shell->current_exec);
		shell->current_exec->return_code = builtin_ftn(shell);
	}
	return (true);
}
