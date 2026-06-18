#include "../minishell.h"

/* Extracted inline comments from expander.c:

- Kelimenin içindeki dış tırnakları (talimat tırnaklarını) siler
- En fazla kendi boyutu kadar yer kaplar
- Eğer çift tırnak içinde değilsek ve tek tırnak gördüysek (Tırnağı atla, duruma geç)
- Eğer tek tırnak içinde değilsek ve çift tırnak gördüysek (Tırnağı atla, duruma geç)
- Normal bir harfse, yeni diziye ekle
- Tırnaklı eski kelimeyi çöpe at
- Tek bir stringin içindeki dolarları (tırnak kurallarına uyarak) bulur ve değiştirir
- 1. Tırnak kapılarını kontrol et
- Çift tırnak içinde değilsek, tek tırnak durumunu tersine çevir
- Tek tırnak içinde değilsek, çift tırnak durumunu tersine çevir
- tırnak algoritması bu şekilde çalışır, tek tırnak açıldıysa çift tırnak etkisizleşir, çift tırnak açıldıysa tek tırnak etkisizleşir, böylece hangi tırnak içinde olduğumuzu doğru şekilde takip edebilirirz
- '"' word '"' gibi bir durumda, ilk '"' gördüğümüzde in_dq = 1 olur, bu sırada in_sq = 0 kalır, ikinci '"' gördüğümüzde in_dq tekrar 0 olur, böylece tırnakların etkisini doğru şekilde yönetebiliriz
- fazla quote durumunu da bu şekilde yönetebiliriz, örneğin: 'abc"def' gibi bir durumda, ilk tek tırnak gördüğümüzde in_sq = 1 olur, bu sırada in_dq = 0 kalır, ikinci tek tırnak gördüğümüzde in_sq tekrar 0 olur, böylece tırnakların etkisini doğru şekilde yönetebiliriz
- '"' gibi bir durumda, ilk çift tırnak gördüğümüzde in_dq = 1 olur, bu sırada in_sq = 0 kalır, ikinci çift tırnak gördüğümüzde in_dq tekrar 0 olur, böylece tırnakların etkisini doğru şekilde yönetebiliriz
- 2. Doları gördük VE tek tırnak içinde DEĞİLSEK işleme başla!
- solunu kes derken örneğin: "$USER" gibi bir durumda, prefix = "" olur, var_name = "USER" olur, suffix = "" olur, böylece "$USER" ifadesini doğru şekilde genişletebiliriz
- --- YENİ: $? KONTROLÜ BURAYA GELDİ ---
- '?' işaretini atla
- --- ESKİ: NORMAL DEĞİŞKEN ($USER vb.) ---
- !!
- Doların sağını kes
- 3. Parçaları birleştir
- !!
- Bellek temizliği (Leak bırakmıyoruz!)
- --- KRİTİK YENİ BÖLÜM: RECURSIVE YERİNE İTERATİF ATLAYIŞ ---
- Eklediğimiz kelimenin uzunluğu kadar ileri zıpla
- Sondaki -1 önemli, çünkü while döngüsünün sonunda i++ var.
- free(var_value);
- Eğer değişken yoksa (boşluğa çevrildiyse) indexi 1 geri çek
- Vagonları (komutları) tek tek gezip, içindeki $ işaretli kelimeleri gerçek değerleriyle değiştirir.

These comments were moved from inside functions to avoid counting as lines inside functions. */

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	int		in_sq;
	int		in_dq;
	char	*new_str;

	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	i = -1;
	j = 0;
	in_sq = 0;
	in_dq = 0;
	while (str[++i])
	{
		if (str[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (str[i] == '"' && !in_sq)
			in_dq = !in_dq;
		else
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

char *get_expand_var_value(t_expand *exp, char *str, int *i, char **env)
{
	int	j;

	j = *i + 1;
	if (str[j] == '?')
	{
		j++;
		exp->var_value = ft_itoa(g_exit_status);
	}
	else
	{
		while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
			j++;
		exp->var_name = ft_substr(str, *i + 1, j - (*i + 1));
		exp->env_val = get_env_value(exp->var_name, env);
		exp->var_value = exp->env_val ? ft_strdup(exp->env_val) : NULL;
	}
	exp->suffix = ft_strdup(str + j);
	return (j > *i + 1 ? (char *)1 : NULL);
}

char	*expand_single_str(char *str, char **env)
{
	t_expand	*expand;
	int			i;
	int			in_sq;
	int			in_dq;

	create_expand(&expand);
	if (!expand)
		return (str);
	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (str[i] == '"' && !in_sq)
			in_dq = !in_dq;
		if (str[i] == '$' && !in_sq && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
			str = execute_expand(str, &i, expand, env);
		i++;
	}
	free_tokens_for_expand(expand);
	return (str);
}

void	expand_cmds(t_cmd *cmds, char **env)
{
	t_cmd *tmp_cmd;
	int i;

	tmp_cmd = cmds;
	while (tmp_cmd != NULL)
	{
		i = 0;
		while (tmp_cmd->args[i] != NULL)
		{
			tmp_cmd->args[i] = expand_single_str(tmp_cmd->args[i], env);
			tmp_cmd->args[i] = remove_quotes(tmp_cmd->args[i]);
			i++;
		}
		tmp_cmd = tmp_cmd->next;
	}
}
