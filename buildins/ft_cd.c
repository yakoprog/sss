#include "../minishell.h"

/* Extracted inline comments from buildins/ft_cd.c:

- 1. Önce mevcut PWD'yi bulup OLDPWD olarak kaydediyoruz
- Önce eskisini silip sonra yenisini ekleyerek "update" yapıyoruz
- 2. İşletim sisteminden şu anki gerçek konumu alıp PWD yapıyoruz
- Başlangıçta 0 kabul ediyoruz
- Tek satırda hata + kod
- Burada argüman ismini de (örn: cd: olmayan_klasor: No such file)
- araya sıkıştırmak gerektiği için mecbur manuel basıyoruz.
- Çünkü print_error sadece iki string (cmd ve msg) alıyor.
- Manuel yazdırdığımız için burada elden güncelledik

These comments were moved from inside functions to avoid counting as lines
inside functions. */

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

/*
** Yardımcı Fonksiyon: Mevcut konumu alıp PWD değişkenini günceller.
*/
static void	sync_pwd(char ***env)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		safe_env_update("PWD=", cwd, env);
}

/*
** Ana Güncelleyici: OLDPWD ve PWD senkronizasyonunu yönetir.
*/
void	update_pwd_oldpwd(char ***env)
{
	char	*old_path;

	// 1. OLDPWD Güncelleme
	old_path = get_env_value("PWD", *env);
	if (old_path)
		safe_env_update("OLDPWD=", old_path, env);
	else
		safe_env_update("OLDPWD=", "", env);

	// 2. PWD Güncelleme
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
