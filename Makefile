BOOT_SRC = ./src/boot/boot.asm
KERNEL_ASM_SRC = ./src/kernel.asm

BOOT_BIN = ./bin/boot.bin
KERNEL_ASM_O = ./build/kernel.asm.o
KERNEL_BIN = ./bin/kernel.bin

KERNEL_SRC = ./src/kernel.c
KERNEL_O = ./build/kernel.o

KERNEL_FULL_O = ./build/kernelfull.o

OS_BIN = ./bin/os.bin

FILES = $(KERNEL_ASM_O) $(KERNEL_O)

INCLUDES = -I./include
FLAGS = -g -ffreestanding -falign-jumps -falign-loops -falign-functions -falign-labels -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-functions -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: $(BOOT_BIN) $(KERNEL_BIN)
	rm -rf $(OS_BIN)
	dd if=$(BOOT_BIN) >> $(OS_BIN)
	dd if=$(KERNEL_BIN) >> $(OS_BIN)
	dd if=/dev/zero bs=512 count=100 >> $(OS_BIN)

$(KERNEL_BIN): $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o $(KERNEL_FULL_O)
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o $(KERNEL_BIN) -ffreestanding -O0 -nostdlib $(KERNEL_FULL_O)

$(BOOT_BIN): $(BOOT_SRC)
	nasm -f bin $(BOOT_SRC) -o $(BOOT_BIN)

$(KERNEL_ASM_O): $(KERNEL_ASM_SRC)
	nasm -f elf -g $(KERNEL_ASM_SRC) -o $(KERNEL_ASM_O)

$(KERNEL_O): $(KERNEL_SRC)
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(KERNEL_SRC) -o $(KERNEL_O)

clean:
	rm -rf $(BOOT_BIN)
	rm -rf $(KERNEL_BIN)
	rm -rf $(OS_BIN)
	rm -rf $(KERNEL_FULL_O)
	rm -rf $(FILES)

