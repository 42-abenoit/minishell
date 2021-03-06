/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 16:52:06 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/11 18:53:02 by abenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <string.h>

# include "minishell.h"

enum		e_retcode
{
	_EXIT_CODE_ = 2,
	ARG_ERR,
	EXIT_NAN,
	_ERRNO_MSG_,
	MALLOC_ERR,
	WRITE_ERR,
	GNL_ERR,
	INVALID_CD_PATH,
	INVALID_CD_NEW_PATH,
	INVALID_PWD_PATH,
	INVALID_PATH_DIR,
	FD_ERROR,
	EXT_CMD_ERROR,
	FORK_ERROR,
	WAIT_ERROR,
	_EXEC_ERROR_,
	AMBIG_REDIR,
	CMD_NOT_FOUND,
	INVALID_EXP_ID,
	INVALID_UNS_ID,
	HOME_NOT_SET,
	CD_ARG_ERR,
	EXIT_ARG_ERR,
	_PARSING_ERROR_,
	SQUOTE_MISSING,
	DQUOTE_MISSING,
	ESCAPE_NL,
	REDIR_PATH_INVALID,
	REDIR_PATH_MISSING,
	EMPTY_CMD,
};

/*
**	ft_error.c
*/

int		exit_trigger(int ret, t_xe *xe);
int		clean_and_exit(int ret, t_xe *xe);
int		ft_error(int ret, t_xe *xe);
int		error_and_exit(enum e_retcode ret, t_xe *xe);

/*
**	ft_error.c
*/

int		parsing_error(int err_code, t_xe *xe);
int		exec_error(int err_code, t_xe *xe);
int		err_output(int err_code, t_xe *xe);

#endif
