.PHONY: client server

INCLUDE_DIR		:= include
CLIENT_FILES 	:= $(wildcard client/*.c)
SERVER_FILES	:= $(wildcard server/*.c)
UTILS_FILES		:= $(wildcard utils/*.c)

client:
	@gcc $(CLIENT_FILES) $(UTILS_FILES) -o game -lraylib -lm -ldl -lpthread -lGL -lX11 -I$(INCLUDE_DIR)

server:
	@gcc $(SERVER_FILES) $(UTILS_FILES) -o serv -Iinclude


