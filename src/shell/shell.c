#include "minishell.h"

void	init_shell(t_shell *shell, t_env_handler *env_handler, char **pwd,
	int prev_retcode)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->env_handler = env_handler;
	shell->current_exec = &shell->exec;
	shell->global_exec.return_code = prev_retcode;
	shell->pwd = pwd;
	return ;
}

int	destroy_shell(t_shell *shell)
{
	shell_close_saved_stdio(shell);
	delete_xbuffer(shell->generated_args_buffer);
	delete_xbuffer(shell->word_buffer);
	destroy_xarray(&shell->redir_args_array);
	destroy_xarray(&shell->argv_array);
	exec_close_redirect_files(shell->current_exec);
	exec_close_redirect_files(&shell->global_exec);
	if (shell->pipeline_execs)
		shell_close_pipes_and_files(shell);
	return (shell->global_exec.return_code);
}

bool	shell_save_stdio(t_shell *shell)
{
	if (shell->current_exec->redir_infile_fd > 0)
		shell->save_stdin_fd = dup(0);
	if (shell->save_stdin_fd == -1)
		return (write_error_strs(2, "dup:", ""), false);
	if (shell->current_exec->redir_outfile_fd > 0)
		shell->save_stdout_fd = dup(1);
	if (shell->save_stdout_fd == -1)
		return (write_error_strs(2, "dup:", ""), false);
	return (true);
}

bool	shell_restore_stdio(t_shell *shell)
{
	if (shell->save_stdin_fd)
	{
		if (dup2(shell->save_stdin_fd, 0) == -1)
			return (write_error_strs(2, "dup2:", ""), false);
		close(shell->save_stdin_fd);
		shell->save_stdin_fd = 0;
	}
	if (shell->save_stdout_fd)
	{
		if (dup2(shell->save_stdout_fd, 1) == -1)
			return (write_error_strs(2, "dup2:", ""), false);
		close(shell->save_stdout_fd);
		shell->save_stdout_fd = 0;
	}
	return (true);
}

void	shell_close_saved_stdio(t_shell *shell)
{
	if (shell->global_exec.pid)
	{
		if (shell->echo_fd)
			close(shell->echo_fd);
		shell->echo_fd = 0;
	}
	if (shell->save_stdin_fd)
		close(shell->save_stdin_fd);
	if (shell->save_stdout_fd)
		close(shell->save_stdout_fd);
	shell->save_stdin_fd = 0;
	shell->save_stdout_fd = 0;
}
