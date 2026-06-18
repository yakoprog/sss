#include "../minishell.h"

/* Extracted inline comments from buildins/builtins.c:

- 1. SÜZGEÇ: Komut gömülü mü yoksa dış sistem komutu mu?
- Eğer hiçbiri değilse 0 döndür (ls, grep vs. demektir)
- 2. ÇALIŞTIRICI: Eğer gömülüyse doğru fonksiyonu çağır
- <--- DİKKAT: Artık char ***env alıyor
- --- PWD KOMUTU ---
- --- ENV KOMUTU ---
- --- EXIT KOMUTU ---
- --- CD KOMUTU ---
- --- ECHO KOMUTU ---
- --- EXPORT KOMUTU (YENİ EKLENDİ) ---
- --- UNSET KOMUTU (YENİ EKLENDİ) ---

These comments were moved from inside functions to avoid counting as lines inside functions. */

int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "pwd", 4) == 0) 
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0) 
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0) 
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0) 
		return (1);
	if (ft_strncmp(cmd, "echo", 5) == 0) 
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0) 
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0) 
		return (1);
	return (0);
}

void execute_builtin(t_cmd *cmd, char ***env)
{
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		ft_env(env);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		ft_exit(cmd);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		ft_cd(cmd, env);
	else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		ft_export(cmd, env);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		ft_unset(cmd, env);
}
