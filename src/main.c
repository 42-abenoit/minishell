/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:47:33 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:48:17 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error.h"
#include "parsing.h"
#include "execution.h"
#include "builtins.h"

static int	get_input(char **line, t_xe *xe)
{
	int		ret;

	if (isatty(STDIN_FILENO)) // temp pour le testeur
	{
		if (ft_putstr_fd(FT_PS1, STDOUT_FILENO) != WRITE_SUCCESS)
			return (WRITE_ERR);
	}
	ret = get_next_line(STDIN_FILENO, line);
	if (ret == ERROR)
		return (GNL_ERR);
	else if (ret == 0)
	{
		free(*line);
		xe->flags -= RUN;
	}
	return (SUCCESS);
}

static int	main_loop(t_xe *xe)
{
	int			ret;
	char		*line;

	ret = get_input(&line, xe);
	if (ret == SUCCESS && xe->flags == RUN)
	{
		xe->line = line;
		ret = check_syntax(line);
		if (ret == SUCCESS)
			ret = handle_execution(xe, STDIN_FILENO, 0);
		free(line);
		ret = ft_error(ret, xe);
	}
	return (ret);
}

int			exec_env_init(t_xe *xe, char **env_source)
{
	char	*shlvl;
	int		tmp;
	int		ret;

	xe->env = dup_str_array(env_source);
	if (xe->env == NULL)
		return (MALLOC_ERR);
	shlvl = get_var_value(xe->env, SHLVL_STR, ft_strlen(SHLVL_STR));
	if (shlvl == NULL)
		return (MALLOC_ERR);
	tmp = ft_atoi(shlvl);
	free(shlvl);
	tmp += 1;
	shlvl = ft_itoa(tmp);
	if (shlvl == NULL)
		return (MALLOC_ERR);
	ret = env_replace_var(SHLVL_STR, shlvl, xe);
	free(shlvl);
	if (ret != SUCCESS)
		return (MALLOC_ERR);
	xe->backup_stdin = dup(STDIN_FILENO);
	xe->backup_stdout = dup(STDOUT_FILENO);
	xe->flags = RUN;
	return (SUCCESS);
}

int			main(int argc, char **argv, char **env_source)
{
	int		ret;
	t_xe	*xe;

	(void)argv;
	ret = SUCCESS;
	xe = (t_xe *)malloc(sizeof(t_xe));
	if (xe == NULL)
		return (error_and_exit(MALLOC_ERR, xe));
	ft_bzero(xe, sizeof(t_xe));
	if (argc > 1)
		ret = ARG_ERR;
	else
	{
		signal_handler();
		ret = exec_env_init(xe, env_source);
		if (ret != SUCCESS)
			return (error_and_exit(ret, xe));
		while (xe->flags & RUN)
		{
			xe->flags = RUN;
			ret = main_loop(xe);
		}
	}
	return (clean_and_exit(ret, xe));
}
