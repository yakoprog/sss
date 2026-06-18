#include "../minishell.h"

/* Extracted inline comments from buildins/env_manager.c:

- Eğer satır "HOME=" ile başlıyorsa...
- '=' işaretinden sonrasını döndür
- Yeni bir değişkeni diziye ekler ve genişletilmiş yeni diziyi döndürür
- 1. Eskisinin boyutunu bul
- 2. Yeni dizi için yer aç (+2 -> Biri yeni değişken, biri en sondaki NULL için)
- 3. Eski elemanları yeni eve kopyala
- 4. Yeni misafiri ekle ve kapıyı kapat
- 5. Eski evi (diziyi) tamamen yık ki hafıza sızıntısı (leak) olmasın!
- Yeni evin adresini döndür
- Sistemin çevre değişkenlerini alıp, müdahale edebileceğimiz dinamik bir kopyasını üretir
- 1. Önce kaç tane değişken olduğunu say
- 2. Yeni dizi için yer ayır (+1 NULL için)
- 3. Her bir satırı ft_strdup ile kopyala
- Diziyi kapat
- İstenilen değişkeni silip, daraltılmış yeni diziyi döndürür
- 1. Eleman sayısını bul
- 2. Yeni dizi için yer aç (Biri silineceği için count kadar yer açmak yeterli)
- 3. İstenmeyen misafir HARİÇ her şeyi kopyala
- Eğer hedef kelimeyse VE hemen ardında '=' varsa, kopyalamadan ATLA!
- Yeni diziyi kapat
- 4. Eski evi yık ki sızıntı (leak) olmasın

These comments were moved from inside functions to avoid counting as lines inside functions. */

char *get_env_value(char *key, char **envp)
{
	int i;
	int key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
		{
			return (envp[i] + key_len + 1);
		}
		i++;
	}
	return (NULL);
}

char **copy_env(char **envp)
{
	int i;
	char **new_env;

	i = 0;
	while (envp[i] != NULL)
		i++;

	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);

	i = 0;
	while (envp[i] != NULL)
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
