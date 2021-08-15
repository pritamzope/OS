# assembler
ASM = /usr/bin/nasm
# compiler
CC = /usr/bin/gcc
# linker
LD = /usr/bin/ld
# grub iso creator
GRUB = /usr/bin/grub-mkrescue
# sources
SRC = src
ASM_SRC = $(SRC)/asm
# objects
OBJ = obj
ASM_OBJ = $(OBJ)/asm
CONFIG = ./config
OUT = out
INC = ./include
INCLUDE=-I$(INC)

MKDIR= mkdir -p
CP = cp -f
DEFINES=

# assembler flags
ASM_FLAGS = -f elf32
# compiler flags
CC_FLAGS = $(INCLUDE) $(DEFINES) -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra
# linker flags, for linker add linker.ld file too
LD_FLAGS = -m elf_i386 -T $(CONFIG)/linker.ld -nostdlib

# target file to create in linking
TARGET=$(OUT)/Console.bin

# iso file target to create
TARGET_ISO=$(OUT)/Console.iso
ISO_DIR=$(OUT)/isodir

OBJECTS=$(ASM_OBJ)/entry.o\
		$(OBJ)/io_ports.o $(OBJ)/vga.o\
		$(OBJ)/string.o $(OBJ)/console.o\
		$(OBJ)/kernel.o


all: $(OBJECTS)
	@printf "[ linking... ]\n"
	$(LD) $(LD_FLAGS) -o $(TARGET) $(OBJECTS)
	grub-file --is-x86-multiboot $(TARGET)
	@printf "\n"
	@printf "[ building ISO... ]\n"
	$(MKDIR) $(ISO_DIR)/boot/grub
	$(CP) $(TARGET) $(ISO_DIR)/boot/
	$(CP) $(CONFIG)/grub.cfg $(ISO_DIR)/boot/grub/
	$(GRUB) -o $(TARGET_ISO) $(ISO_DIR)
	rm -f $(TARGET)

$(ASM_OBJ)/entry.o : $(ASM_SRC)/entry.asm
	@printf "[ $(ASM_SRC)/entry.asm ]\n"
	$(ASM) $(ASM_FLAGS) $(ASM_SRC)/entry.asm -o $(ASM_OBJ)/entry.o
	@printf "\n"

$(OBJ)/io_ports.o : $(SRC)/io_ports.c
	@printf "[ $(SRC)/io_ports.c ]\n"
	$(CC) $(CC_FLAGS) -c $(SRC)/io_ports.c -o $(OBJ)/io_ports.o
	@printf "\n"

$(OBJ)/vga.o : $(SRC)/vga.c
	@printf "[ $(SRC)/vga.c ]\n"
	$(CC) $(CC_FLAGS) -c $(SRC)/vga.c -o $(OBJ)/vga.o
	@printf "\n"

$(OBJ)/string.o : $(SRC)/string.c
	@printf "[ $(SRC)/string.c ]\n"
	$(CC) $(CC_FLAGS) -c $(SRC)/string.c -o $(OBJ)/string.o
	@printf "\n"

$(OBJ)/console.o : $(SRC)/console.c
	@printf "[ $(SRC)/console.c ]\n"
	$(CC) $(CC_FLAGS) -c $(SRC)/console.c -o $(OBJ)/console.o
	@printf "\n"

$(OBJ)/kernel.o : $(SRC)/kernel.c
	@printf "[ $(SRC)/kernel.c ]\n"
	$(CC) $(CC_FLAGS) -c $(SRC)/kernel.c -o $(OBJ)/kernel.o
	@printf "\n"

clean:
	rm -f $(OBJ)/*.o
	rm -f $(ASM_OBJ)/*.o
	rm -rf $(OUT)/*
