#include "minishell.h"

volatile bool	g_sig = false;

static void	sigint_handler(int signum)
{
	(void)signum;
	if (waitpid(-1, 0, 0) < 0)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_sig = true;
}

void	init_sig_handler(bool is_parent)
{
	static struct sigaction	sa_sigint;
	static struct sigaction	sa_sigquit;

	sa_sigint.sa_flags = SA_RESTART;
	sa_sigquit.sa_flags = SA_RESTART;
	sigemptyset(&sa_sigint.sa_mask);
	sigemptyset(&sa_sigquit.sa_mask);
	sa_sigint.sa_handler = &sigint_handler;
	if (is_parent)
		sa_sigquit.sa_handler = SIG_IGN;
	else
		sa_sigquit.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa_sigint, NULL);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

static void	heredoc_sigint_handler(int signum)
{
	t_shell_heredoc	*gethdoc_process;

	(void)signum;
	gethdoc_process = heredoc_sig(NULL);
	destroy_env_handler(gethdoc_process->shell->env_handler);
	destroy_parser(gethdoc_process->shell->parser);
	if (gethdoc_process->exp_string)
		destroy_xstring(gethdoc_process->exp_string);
	close(gethdoc_process->pipe_fds[PIPE_WRITE_END_FD]);
	destroy_shell(gethdoc_process->shell);
	free(*gethdoc_process->shell->pwd);
	exit(130);
}

t_shell_heredoc	*heredoc_sig(t_shell_heredoc *heredoc)
{
	static t_shell_heredoc	*gethdoc_process = NULL;
	struct sigaction		sa_sigint;
	struct sigaction		sa_sigquit;

	if (!heredoc)
		return (gethdoc_process);
	gethdoc_process = heredoc;
	sa_sigint.sa_flags = SA_RESTART;
	sa_sigquit.sa_flags = SA_RESTART;
	sigemptyset(&sa_sigint.sa_mask);
	sigemptyset(&sa_sigquit.sa_mask);
	sa_sigint.sa_handler = &heredoc_sigint_handler;
	sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_sigint, NULL);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
	return (NULL);
}
