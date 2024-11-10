# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/09 06:31:36 by teando            #+#    #+#              #
#    Updated: 2024/11/10 14:18:27 by teando           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER_NAME	:= server
CLIENT_NAME	:= client
SERVER_BONUS_NAME	:= server_bonus
CLIENT_BONUS_NAME	:= client_bonus

LIBFT_PATH = ./libft
CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIC
LIBFT = $(LIBFT_PATH)/libft.a

ROOT_DIR	:= .
OUT_DIR		:= $(ROOT_DIR)/obj
SRC_DIR		:= $(ROOT_DIR)/mandatory
BONUS_DIR	:= $(ROOT_DIR)/bonus
INCLUDES = -I./inc -I$(LIBFT_PATH)

SERVER_SRC	:= $(SRC_DIR)/server.c
CLIENT_SRC	:= $(SRC_DIR)/client.c
SERVER_OBJ	:= $(OUT_DIR)/server.o
CLIENT_OBJ	:= $(OUT_DIR)/client.o

SERVER_BONUS_SRC	:= $(BONUS_DIR)/server_bonus.c
CLIENT_BONUS_SRC	:= $(BONUS_DIR)/client_bonus.c
SERVER_BONUS_OBJ	:= $(OUT_DIR)/server_bonus.o
CLIENT_BONUS_OBJ	:= $(OUT_DIR)/client_bonus.o

all: $(LIBFT) $(SERVER_NAME) $(CLIENT_NAME)

bonus: $(LIBFT) $(SERVER_BONUS_NAME) $(CLIENT_BONUS_NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

$(SERVER_NAME):$(SERVER_OBJ) $(LIBFT) 
	$(CC) $(CFLAGS) -o $@ $^
 
$(CLIENT_NAME): $(CLIENT_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_BONUS_NAME):$(SERVER_BONUS_OBJ) $(LIBFT) 
	$(CC) $(CFLAGS) -o $@ $^
 
$(CLIENT_BONUS_NAME): $(CLIENT_BONUS_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OUT_DIR)/%_bonus.o: $(BONUS_DIR)/%_bonus.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -rf $(SERVER_OBJ) $(CLIENT_OBJ) $(OUT_DIR)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -rf $(SERVER_NAME) $(CLIENT_NAME) $(SERVER_BONUS_NAME) $(CLIENT_BONUS_NAME)
	make -C $(LIBFT_PATH) fclean

norm: $(SERVER_SRC) $(CLIENT_SRC) $(SERVER_BONUS_SRC) $(CLIENT_BONUS_SRC)
	@norminette $^

re: fclean all

.PHONY: all clean fclean re norm