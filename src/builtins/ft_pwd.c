#include "builtins.h"

int		ft_pwd(char **args, t_xe *xe)
{
	char	*buf;

	(void)args;
	(void)xe;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		xe->stat_loc = 1;
		return (INVALID_PWD_PATH); // other code!
	}
	if (ft_putendl_fd(buf, STDOUT_FILENO) != WRITE_SUCCESS)
	{
		free(buf);
		return (WRITE_ERR);
	}
	free(buf);
	xe->stat_loc = 0;
	return (SUCCESS);
}