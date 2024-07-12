FLAGS := -std=c99
FLAGS += -g
FLAGS += -pedantic-errors
FLAGS += -Wall
FLAGS += -Wextra
FLAGS += -Werror=vla
FLAGS += -Werror
FLAGS += -O0

COMM := gcc $(FLAGS)

OBJ_FILES := chip8_disassemble.o helpers_disasm.o

chip8_disassemble: $(OBJ_FILES)
	$(COMM) -o chip8_disassemble $(OBJ_FILES)
	
chip8_disassemble.o: chip8_disassemble.c
	$(COMM) -c chip8_disassemble.c

helpers_disasm.o: helpers_disasm.c
	$(COMM) -c helpers_disasm.c


.PHONY: clean
clean:
	rm -i *.o
	rm -i chip8_disassemble
