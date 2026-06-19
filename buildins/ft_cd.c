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
	char	*key_no_eq;
	int		len;

	env_str = ft_strjoin(key, value);
	if (!env_str)
		return ;
	len = ft_strlen(key);
	if (len > 0 && key[len - 1] == '=')
		len--;
	key_no_eq = ft_substr(key, 0, len);
	if (!key_no_eq)
	{
		free(env_str);
		return ;
	}
	*env = export_remove(key_no_eq, *env);
	*env = export_add(env_str, *env);
	free(key_no_eq);
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
	char	*old_path_copy;

	old_path = get_env_value("PWD", *env);
	if (old_path)
		old_path_copy = ft_strdup(old_path);
	else
		old_path_copy = ft_strdup("");
	if (old_path_copy)
	{
		safe_env_update("OLDPWD=", old_path_copy, env);
		free(old_path_copy);
	}
	sync_pwd(env);
}

static int	ft_cd_no_arg(char ***env)
{
	char	*home;

	home = get_env_value("HOME", *env);
	if (home == NULL)
	{
		print_error("cd", "HOME not set", 1);
		return (1);
	}
	if (chdir(home) != 0)
	{
		print_error("cd", strerror(errno), 1);
		return (1);
	}
	return (0);
}

void	ft_cd(t_cmd *cmd, char ***env)
{
	shell->exit_status = 0;
	if (cmd->args[1] != NULL && cmd->args[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return ;
	}
	if (cmd->args[1] == NULL)
		shell->exit_status = ft_cd_no_arg(env);
	else if (chdir(cmd->args[1]) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		shell->exit_status = 1;
	}
	if (shell->exit_status == 0)
		update_pwd_oldpwd(env);
}
