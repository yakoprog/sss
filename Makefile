NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
LIBFT   = ./libft/libft.a
GET		= ./get_next_line/libgnl.a
FTPRINTF = ./ft_printf/libftprintf.a
LIBS    = -lreadline -L./libft -lft -L./get_next_line -lgnl -L./ft_printf -lftprintf


SRC     = main.c \
./lexer_parser/lexer.c \
./lexer_parser/parser.c \
./lexer_parser/free_tokens.c \
./lexer_parser/redirect_check.c \
./lexer_parser/parser_utils.c \
./lexer_parser/parser_check.c \
./lexer_parser/token_create.c \
./execution/command_parsed.c \
./execution/after_lexer.c \
./execution/ft_free.c 		\
./execution/pipe_connection.c \
./execution/ft_multible_pipe.c \
./buildins/builtins.c \
./buildins/env_manager.c \
./buildins/ft_cd.c \
./buildins/ft_echo.c \
./buildins/ft_env.c \
./buildins/ft_exit.c \
./buildins/ft_export.c \
./buildins/ft_pwd.c \
./buildins/ft_unset.c \
signals.c \
./error_and_utils/utils.c \
./error_and_utils/error_utils.c \
./expander/expander.c \
./expander/expander_utils.c \
./expander/expand_cmd.c \
./buildins/env_utils.c 


OBJ     = $(SRC:.c=.o)

RM = rm -f

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(GET) $(FTPRINTF)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

$(LIBFT):
	echo "==> building libft"
	@$(MAKE) -C ./libft

$(GET):
	echo "==> building get_next_line"
	@$(MAKE) -C ./get_next_line

$(FTPRINTF):
	echo "==> building ft_printf"
	@$(MAKE) -C ./ft_printf

clean:
	@$(MAKE) -C ./libft clean
	@$(MAKE) -C ./get_next_line clean
	@$(MAKE) -C ./ft_printf clean
	$(RM) $(OBJ)

fclean: clean
	@$(MAKE) -C ./libft fclean
	@$(MAKE) -C ./get_next_line fclean
	@$(MAKE) -C ./ft_printf fclean
	$(RM) $(NAME)

test: all
	@$(MAKE) -C ./get_next_line test || true
	@$(MAKE) clean

re: fclean all

.PHONY: all clean fclean re test $(LIBFT) $(GET) $(FTPRINTF)