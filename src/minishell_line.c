#include "minishell.h"

static bool	is_only_white_space(char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (false);
		str++;
	}
	return (true);
}

// If echo_fd not 0, redirect readline echo info this file instead of stdout
// Use /dev/nul to disable echo
bool	readline_ext(char **out_input, char *prompt, int echo_fd)
{
	int	save_stdout;

	if (!echo_fd)
		return (*out_input = readline(prompt), true);
	*out_input = NULL;
	save_stdout = dup(STDOUT_FILENO);
	if (save_stdout == -1)
		return (write_error_strs(2, "dup:", ""), false);
	if (dup2(echo_fd, STDOUT_FILENO) == -1)
		return (close(save_stdout), write_error_strs(2, "dup2:", ""), false);
	*out_input = readline(prompt);
	if (dup2(save_stdout, STDOUT_FILENO) == -1)
		return (close(save_stdout), write_error_strs(2, "dup2:", ""), false);
	return (close(save_stdout), true);
}

char	*minishell_get_line(t_minishell *minishell)
{
	char	*input;
	char	*prompt;

	input = NULL;
	while (!input)
	{
		prompt = NULL;
		if (get_prompt(&prompt, minishell))
			return (NULL);
		if (!readline_ext(&input, prompt, minishell->echo_fd))
			return (free(prompt), NULL);
		free(prompt);
		if (!input)
			return (NULL);
		if (is_only_white_space(input))
		{
			free(input);
			input = NULL;
			continue ;
		}
	}
	if (!minishell->echo_fd)
		add_history(input);
	return (input);
}

static bool	_ret(t_minishell *minishell, t_shell *shell, t_parser *parser)
{
	minishell->prev_retcode = destroy_shell(shell);
	destroy_parser(parser);
	return (shell->global_exec.pid || (!shell->global_exec.pid && shell->exit));
}

// Returns true if exit is requested
bool	minishell_exec_line(t_minishell *minishell, char *input)
{
	t_parser	parser;
	t_shell		shell;

	if (!init_parser(&parser))
		return (write_error_strs(2, APP_NAME, ": ", ""), false);
	parser.get_heredoc = shell_get_heredoc;
	parser.app_info = (void *)&shell;
	init_shell(&shell, &minishell->env_handler,
		&minishell->pwd, minishell->prev_retcode);
	shell.parser = &parser;
	shell.echo_fd = minishell->echo_fd;
	if (!parser_parse(&parser, input))
	{
		minishell_write_parser_errors(minishell, &parser);
		shell.global_exec.return_code = minishell->prev_retcode;
	}
	else
	{
		shell.ast = &parser.ast;
		shell_exec_subtree(&shell, shell.ast->nodes[0].left_node);
		if (shell.global_exec.pid == 0)
			shell_clean_tree_files(&shell, shell.ast->nodes[0].left_node);
	}
	return (_ret(minishell, &shell, &parser));
}
