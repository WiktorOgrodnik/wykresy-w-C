CC = gcc
SRC_DIR = .
INC_DIR = .
OBJ_DIR = .
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wno-unused-parameter
GTK = `pkg-config --cflags --libs gtk+-3.0` -lm
LIBS = `pkg-config --libs gtk+-3.0` -lm
NAME = wykresy

OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/eval.o $(OBJ_DIR)/draw.o $(OBJ_DIR)/convert.o $(OBJ_DIR)/ui.o

all: $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c -I$(INC_DIR) $< -o $@ $(GTK)

clean:
	@rm -f $(NAME) $(OBJS)