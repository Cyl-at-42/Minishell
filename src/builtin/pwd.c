#include "builtin.h"

int	builtin_pwd(t_shell *shell)
{
	write(1, *shell->pwd, ft_strlen(*shell->pwd));
	write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
