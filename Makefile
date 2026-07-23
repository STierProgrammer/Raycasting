.PHONY: client server

INCLUDE_DIR		:= include
CLIENT_FILES 	:= $(shell find client -name "*.c")
SERVER_FILES	:= $(shell find server -name "*.c")
UTILS_FILES		:= $(shell find utils -name "*.c")

client:
	@gcc $(CLIENT_FILES) $(UTILS_FILES) -o game -lraylib -lm -ldl -lpthread -lGL -lX11 -I$(INCLUDE_DIR)

server:
	@gcc $(SERVER_FILES) $(UTILS_FILES) -o serv -Iinclude


