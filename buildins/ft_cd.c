/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:34:17 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:34:17 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	safe_env_update(char *key, char *value, char ***env)
{
	char	*env_str;

	env_str = ft_strjoin(key, value);
	if (!env_str)
		return ;
	*env = export_remove(key, *env);
	*env = export_add(env_str, *env);
	free(env_str);
}

static void	sync_pwd(char ***env)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		safe_env_update("PWD=", cwd, env);
}

void	update_pwd_oldpwd(char ***env)
{
	char	*old_path;

	old_path = get_env_value("PWD", *env);
	if (old_path)
		safe_env_update("OLDPWD=", old_path, env);
	else
		safe_env_update("OLDPWD=", "", env);
	sync_pwd(env);
}

void	ft_cd(t_cmd *cmd, char ***env)
{
	char	*home;

	g_exit_status = 0;
	if (cmd->args[1] == NULL)
	{
		home = get_env_value("HOME", *env);
		if (home == NULL)
			print_error("cd", "HOME not set", 1);
		else if (chdir(home) != 0)
			print_error("cd", strerror(errno), 1);
	}
	else if (chdir(cmd->args[1]) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		g_exit_status = 1;
	}
	if (g_exit_status == 0)
		update_pwd_oldpwd(env);
}
