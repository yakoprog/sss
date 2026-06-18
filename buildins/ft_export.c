#include "../minishell.h"

/* Extracted inline comments from buildins/ft_export.c:

- 0. index "export" kelimesi olduğu için 1'den başlıyoruz
- Başlangıçta her şey yolunda kabul et
- Eğer sadece 'export' yazıldıysa (Bunu env'yi alfabetik basmak için ayırıyoruz)
- Burada sadece export yazıldığındaki işlemi yaparsın (declare -x listesi)
- Bütün argümanları tek tek gez (örn: export 1HATALI=a DOGRU=b)
- Kapıdaki güvenlik kural ihlali yakaladı!
- DİKKAT: return atıp çıkmıyoruz! Sadece bunu atlayıp diğerlerine (DOGRU=b) geçmesi için devam ediyoruz.
- Kurallara uyuyor, işçiye (export_add) gönder!

These comments were moved from inside functions to avoid counting as lines inside functions. */

void	ft_export(t_cmd *cmd, char ***env)
{
	int i;

	g_exit_status = 0;

	if (cmd->args[1] == NULL)
	{
		return ;
	}

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (is_valid_env_name(cmd->args[i]) == 0)
		{
			export_error("export", cmd->args[i]); 
		}
		else
		{
			*env = export_add(cmd->args[i], *env);
		}
		i++;
	}
}
