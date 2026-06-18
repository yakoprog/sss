#include "../minishell.h"

/* Extracted inline comments from error_and_utils/utils.c:

- 1. Çift yıldızlı çevre değişkeni dizisini komple temizler
- Önce içindeki bütün kelimeleri (satırları) sil
- Sonra dizinin (evin) kendisini komple yık
- 2. Tesisatçının işi bittikten sonra vagonları (komut listesini) temizler
- O anki vagonu tut
- Treni bir sonrakine kaydır
- A) Vagonun içindeki kelimeleri (args) sil
- Kelime kutusunu sil
- B) Vagonun kendisini çöpe at
- 1. Mevcut SHLVL değerini bul
- 2. Eğer değer varsa sayıya çevir ve 1 artır, yoksa 1 kabul et
- 3. Sayıyı tekrar yazıya ("2" gibi) çevir
- 4. "SHLVL=" ile sayıyı birleştir ("SHLVL=2" yap)
- 5. Eskisini sil, yenisini listeye ekle
- 6. Çöpleri (Leak) temizle
- 1. Bekçi: Kapanmamış Tırnak Kontrolü
- 258 Bash'in resmi "Syntax Error" çıkış kodudur.
- Hatalı
- Başarılı
- 2. Bekçi: Yanyana Yanlış İşaret Kullanımı Kontrolü
- Kural 1: İlk komut PIPE (|) ile başlayamaz
- Kural 2: Yönlendirmeden (<, >, <<, >>) hemen sonra WORD (kelime) gelmek ZORUNDA
- Kural 3: Art arda iki PIPE (| |) veya en sonda PIPE (ls |) olamaz

These comments were moved from inside functions to avoid counting as lines inside functions. */

void	free_env(char **env)
{
	int i;

	if (!env)
		return;
	i = 0;
	while (env[i] != NULL)
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void free_cmds(t_cmd *cmds)
{
	t_cmd *tmp;
	int i;

	while (cmds != NULL)
	{
		tmp = cmds;
		cmds = cmds->next;

		if (tmp->args != NULL)
		{
			i = 0;
			while (tmp->args[i] != NULL)
			{
				free(tmp->args[i]);
				i++;
			}
			free(tmp->args);
		}
		free(tmp);
	}
}

void	increment_shlvl(char ***env)
{
	char *shlvl_val;
	int shlvl_num;
	char *new_num_str;
	char *new_shlvl_str;

	shlvl_val = get_env_value("SHLVL", *env);
	if (shlvl_val)
		shlvl_num = ft_atoi(shlvl_val) + 1;
	else
		shlvl_num = 1;
	new_num_str = ft_itoa(shlvl_num);
	new_shlvl_str = ft_strjoin("SHLVL=", new_num_str);
	*env = export_remove("SHLVL", *env);
	*env = export_add(new_shlvl_str, *env);
	free(new_num_str);
	free(new_shlvl_str);
}

int	check_quotes(char *str)
{
	int i;
	int quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (quote == 0 && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote == str[i])
			quote = 0;
		i++;
	}
	if (quote != 0)
	{
		print_error(NULL, "syntax error: unclosed quotes", 258);
		return (0);
	}
	return (1);
}

int	check_syntax(t_token *tokens)
{
	t_token *tmp;

	tmp = tokens;
	if (tokens && tokens->type == PIPE)
	{
		print_error(NULL, "syntax error near unexpected token `|'", 258);
		return (0);
	}
	while (tmp)
	{
		if (tmp->type >= REDIR_IN && tmp->type <= APPEND)
		{
			if (!tmp->next || tmp->next->type != WORD)
			{
				print_error(NULL, "syntax error near unexpected token `newline'", 258);
				return (0);
			}
		}
		if (tmp->type == PIPE)
			if (!pipe_error(tmp))
				return (0);
		tmp = tmp->next;
	}
	return (1);
}
