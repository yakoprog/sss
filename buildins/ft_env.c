/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:33:23 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:33:23 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(char ***env)
{
	int	i;

	i = 0;
	while ((*env)[i] != NULL)
	{
		ft_putendl_fd((*env)[i], 1);
		i++;
	}
	shell->exit_status = 0;
}
