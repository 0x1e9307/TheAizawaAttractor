TARGET = Aizawa
CC = gcc

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(shell find $(PREF_SRC) -name "*.c")
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -no-pie -lglut -lGL -lGLU -lm -std=c99

$(PREF_OBJ)%.o: $(PREF_SRC)%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ -std=c99

clean:
	rm -f $(TARGET) $(OBJ)
