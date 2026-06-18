#include "../minishell.h"

/* Extracted inline comments from lexer_parser/lexer.c:

- New_Token function to create a new token
- Not an operator
- build the token and append it to the list
- Lexer structure
- Skip leading whitespace
- Empty input
- input + i yapıyoruz çünkü handle_operator fonksiyonu input'un geri kalanını alacak şekilde çalışıyor, böylece operatörün tam olarak nerede olduğunu ve ne olduğunu anlayabilir
- Handle operators and move index
- Debug için kullanılacak fonksiyon. Proje bitince silincek !!!

These comments were moved from inside functions to avoid counting as lines inside functions. */

int	handle_operator(char *input, t_token **tokens)
{
	t_type	type;
	int		len;

	len = 1;
	if (input[0] == '|')
		type = PIPE;
	else if (input[0] == '<' && input[1] == '<')
	{
		len = 2;
		type = HEREDOC;
	}
	else if (input[0] == '>' && input[1] == '>')
	{
		len = 2;
		type = APPEND;
	}
	else if (input[0] == '<')
		type = REDIR_IN;
	else if (input[0] == '>')
		type = REDIR_OUT;
	else
		return (0);

	operator_create(tokens, type, input, len);
	return (len);
}

int handle_word(char *input, t_token **tokens)
{
	int len;
	int quote;
	int quote_type;

	len = 0;
	quote = 0;
	quote_type = 0;

	while (input[len])
	{
		single_double_none_quote(input, len, &quote_type);
		if (quote == 0 && (input[len] == ' ' || input[len] == '\t' || 
								input[len] == '|' || input[len] == '<' || 
								input[len] == '>'))
		{
			break;
		}
		len++;
	}
	word_create(tokens, input, quote_type, len);
	return (len);
}

void lexer(char *input, t_token **tokens)
{
	int i;

	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == '\0')
			break;
		if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			i += handle_operator(input + i, tokens);
		}
		else
		{
			i += handle_word(input + i, tokens);
		}
	}
}

void free_tokens(t_token *tokens)
{
	t_token *temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}
