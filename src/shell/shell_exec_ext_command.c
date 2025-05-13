#include "minishell.h"

static char	*_shell_find_ext_command_path_search(t_shell *shell, char *cmd_name)
{
	char			*env_path;
	char			*cmd_path;
	char			*res_cmd_path;

	env_path = env_handler_get_env_value(shell->env_handler, "PATH");
	if (!env_path)
		return (NULL);
	cmd_path = find_cmd_in_path_env(cmd_name, env_path);
	if (!cmd_path)
		return (write_error_strs(2, cmd_name, ": command not found\n"),
			shell->current_exec->return_code = 127, NULL);
	res_cmd_path = xbuffer_add_data(shell->generated_args_buffer, cmd_path,
			ft_strlen(cmd_path) + 1);
	free(cmd_path);
	if (access(res_cmd_path, X_OK) == -1)
		return (write_error_strs(5, APP_NAME, ": ", res_cmd_path,
				":", ""), shell->current_exec->return_code = 126, NULL);
	return (res_cmd_path);
}

char	*shell_find_ext_command(t_shell *shell, char *cmd_name)
{
	char	*cmd_path;

	if (!ft_strrchr(cmd_name, '/'))
	{
		cmd_path = _shell_find_ext_command_path_search(shell, cmd_name);
		if (shell->current_exec->return_code != 1)
			return (NULL);
		if (!cmd_path)
			cmd_path = cmd_name;
	}
	else
		cmd_path = cmd_name;
	if (access(cmd_path, F_OK) == -1)
		return (write_error_strs(5, APP_NAME, ": ",
				cmd_path, ":", ""), shell->current_exec->return_code = 127,
			NULL);
	if (access(cmd_path, X_OK) == -1)
		return (write_error_strs(5, APP_NAME, ": ",
				cmd_path, ":", ""), shell->current_exec->return_code = 126,
			NULL);
	return (cmd_path);
}

static bool	_shell_exec_ext_cmd_child(t_shell *shell, char *cmd_path,
	char **argv, char **envp)
{
	if (shell->pipeline_execs)
	{
		if (!exec_redirect_files(&shell->global_exec,
				shell->pipeline_exec_index == 0,
				shell->pipeline_exec_index == shell->pipeline_size - 1))
			return (false);
		exec_close_redirect_files(&shell->global_exec);
		if (!shell_redirect_pipes(shell, shell->current_exec))
			return (false);
		if (!exec_redirect_files(shell->current_exec, true, true))
			return (false);
		shell_close_pipes_and_files(shell);
	}
	else
	{
		if (!exec_redirect_files(&shell->global_exec, true, true))
			return (false);
		exec_close_redirect_files(&shell->global_exec);
		if (!exec_redirect_files(shell->current_exec, true, true))
			return (false);
		exec_close_redirect_files(shell->current_exec);
	}
	envp = env_handler_create_envtable(shell->env_handler);
	execve(cmd_path, argv, envp);
	return (free(envp), write_error_strs(2, "execve:", ""), false);
}

static bool	_shell_execute_ext_command_ret(t_shell *shell, pid_t new_pid)
{
	int	status;

	shell->current_exec->pid = new_pid;
	if (shell->pipeline_execs)
		return (true);
	status = 0;
	waitpid(new_pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			write_error_strs(1, "Quit (core dumped)\n");
			return (shell->current_exec->return_code = 131, true);
		}
	}
	return (shell->current_exec->return_code = WEXITSTATUS(status), true);
}

bool	shell_execute_ext_command(t_shell *shell, char **argv)
{
	char		*cmd_path;
	pid_t		new_pid;
	struct stat	statbuffer;
	char		*envp;

	cmd_path = shell_find_ext_command(shell, argv[0]);
	if (!cmd_path)
		return (true);
	if (stat(cmd_path, &statbuffer) == -1)
		return (true);
	if (S_ISDIR(statbuffer.st_mode))
		return (shell->current_exec->return_code = 126, write_error_strs(4,
				APP_NAME, ": ", cmd_path, ": Is a directory\n"), true);
	new_pid = fork();
	if (new_pid == -1)
		return (write_error_strs(2, "fork:", ""), false);
	if (new_pid == 0)
	{
		shell->global_exec.pid = -1;
		shell_close_saved_stdio(shell);
		init_sig_handler(false);
		return (_shell_exec_ext_cmd_child(shell, cmd_path, argv, &envp), false);
	}
	exec_close_redirect_files(shell->current_exec);
	return (_shell_execute_ext_command_ret(shell, new_pid));
}
