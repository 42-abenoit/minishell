/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blank_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-a <mvidal-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 12:18:10 by mvidal-a          #+#    #+#             */
/*   Updated: 2021/02/01 11:34:22 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			blank_str(char *line)
{
	while (ft_isspace(*line))
		line++;
	if (*line == '\0')
		return (TRUE);
	return (FALSE);
}
