/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:32:18 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:32:18 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(char ***env)
{
	char	cwd[1024];
	char	*pwd_val;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		g_exit_status = 0;
	}
	else
	{
		pwd_val = get_env_value("PWD", *env);
		if (pwd_val)
		{
			printf("%s\n", pwd_val);
			g_exit_status = 0;
		}
		else
			print_error("pwd", strerror(errno), 1);
	}
}
