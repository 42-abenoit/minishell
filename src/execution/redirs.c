/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:15:30 by abenoit           #+#    #+#             */
/*   Updated: 2021/02/11 18:15:33 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int			find_ambig_redir(char **redir_paths, enum e_redir_op *redir_types)
{
	int		i;

	i = 0;
	if (redir_paths == NULL || redir_types == NULL)
		return (FAILURE);
	while (redir_paths[i] != NULL)
	{
		if (redir_types[i] == AMBIG)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

static void	create_fd(char *cur_arg, enum e_redir_op redir,
						int *src_fd, int *redir_fd)
{
	mode_t		mode;
	int			flags;

	mode = 0;
	if (redir == FILEIN)
	{
		*src_fd = STDIN_FILENO;
		flags = O_RDONLY;
	}
	else
	{
		*src_fd = STDOUT_FILENO;
		flags = O_WRONLY | O_CREAT;
		mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
		if (redir == APPEND)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
	}
	*redir_fd = open(cur_arg, flags, mode);
}

int			apply_redir(char *cur_arg, enum e_redir_op redir, t_xe *xe)
{
	int			src_fd;
	int			redir_fd;

	create_fd(cur_arg, redir, &src_fd, &redir_fd);
	if (redir_fd >= 0)
	{
		if (dup2(redir_fd, src_fd) == ERROR)
			return (FD_ERROR);
	}
	else
	{
		xe->stat_loc = 1;
		return (FD_ERROR);
	}
	return (SUCCESS);
}

int			apply_redirs(char **redir_paths, enum e_redir_op *redir_types,
							t_xe *xe)
{
	int		i;
	int		ret;

	i = 0;
	while (redir_paths[i] != NULL)
	{
		ret = apply_redir(redir_paths[i], redir_types[i], xe);
		if (ret != SUCCESS)
			return (ret);
		i++;
	}
	return (SUCCESS);
}
