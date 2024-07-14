FLAGS := -std=c99
FLAGS += -g
FLAGS += -pedantic-errors
FLAGS += -Wall
FLAGS += -Wextra
FLAGS += -Werror=vla
FLAGS += -Werror
FLAGS += -O0

COMM := gcc $(FLAGS)

OBJ_FILES := chip8_emulate.o helpers_emulate.o

chip8_emulate: $(OBJ_FILES)
	$(COMM) -o chip8_emulate $(OBJ_FILES)
	
chip8_emulate.o: chip8_emulate.c
	$(COMM) -c chip8_emulate.c

helpers_emulate.o: helpers_emulate.c
	$(COMM) -c helpers_emulate.c


.PHONY: clean
clean:
	rm -i *.o
	rm -i chip8_emulate
