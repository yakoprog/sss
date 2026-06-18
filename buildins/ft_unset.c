#include "../minishell.h"

/* Extracted inline comments from buildins/ft_unset.c:

- 0. index "unset" olduğu için 1'den (argümanlardan) başlıyoruz
- Başlangıçta her şey yolunda kabul et
- Bütün argümanları tek tek gez (örn: unset DOGRU 1HATALI YANLIS=1)
- 1. Kural: İsim geçerli mi? (is_valid_env_name)
- 2. Kural: İçinde '=' işareti var mı? (ft_strchr)
- Hata basıcıyı bu kez "export" değil "unset" adıyla çağırıyoruz.
- Aynı fonksiyon, çift yönlü kullanım!
- Kurallara uyuyorsa işçiye (export_remove) gönder ve yeni adresi kaydet

These comments were moved from inside functions to avoid counting as lines inside functions. */

void	ft_unset(t_cmd *cmd, char ***env)
{
	int i;

	g_exit_status = 0;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (is_valid_env_name(cmd->args[i]) == 0 || ft_strchr(cmd->args[i], '='))
		{
			export_error("unset", cmd->args[i]);
		}
		else
		{
			*env = export_remove(cmd->args[i], *env);
		}
		i++;
	}
}
