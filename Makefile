# Sources to assemble
BOOT_SRC = ./src/boot/boot.asm
BOOT_BIN = ./bin/boot.bin

KERNEL_ASM_SRC = ./src/kernel.asm
KERNEL_ASM_O = ./build/kernel.asm.o
KERNEL_SRC = ./src/kernel.c
KERNEL_O = ./build/kernel.o
KERNEL_BIN = ./bin/kernel.bin
KERNEL_FULL_O = ./build/kernelfull.o

IDT_ASM_SRC = ./src/idt/idt.asm
IDT_ASM_O = ./build/idt/idt.asm.o
IDT_SRC = ./src/idt/idt.c
IDT_O = ./build/idt/idt.o

MEMORY_SRC = ./src/memory/memory.c
MEMORY_O = ./build/memory/memory.o

IO_ASM_SRC = ./src/io/io.asm
IO_ASM_O = ./build/io/io.asm.o

OS_BIN = ./bin/os.bin
###

# File paths, include paths and compilation flags
FILES = $(KERNEL_ASM_O) $(KERNEL_O) $(IDT_ASM_O) $(IDT_O) $(MEMORY_O) $(IO_ASM_O)
INCLUDES = -I./include
FLAGS = -g -ffreestanding -falign-jumps -falign-loops -falign-functions -falign-labels -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-functions -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
###

# Build scripts
# build os by assembling the boot and the kernel
all: $(BOOT_BIN) $(KERNEL_BIN)
	rm -rf $(OS_BIN)
	dd if=$(BOOT_BIN) >> $(OS_BIN)
	dd if=$(KERNEL_BIN) >> $(OS_BIN)
	dd if=/dev/zero bs=512 count=100 >> $(OS_BIN)

$(BOOT_BIN): $(BOOT_SRC)
	nasm -f bin $(BOOT_SRC) -o $(BOOT_BIN)

$(KERNEL_BIN): $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o $(KERNEL_FULL_O)
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o $(KERNEL_BIN) -ffreestanding -O0 -nostdlib $(KERNEL_FULL_O)
###

# OS components
$(KERNEL_ASM_O): $(KERNEL_ASM_SRC)
	nasm -f elf -g $(KERNEL_ASM_SRC) -o $(KERNEL_ASM_O)

$(KERNEL_O): $(KERNEL_SRC)
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(KERNEL_SRC) -o $(KERNEL_O)

$(IDT_ASM_O): $(IDT_ASM_SRC)
	nasm -f elf -g $(IDT_ASM_SRC) -o $(IDT_ASM_O)

$(IDT_O): $(IDT_SRC)
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(IDT_SRC) -o $(IDT_O)

$(MEMORY_O): $(MEMORY_SRC)
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(MEMORY_SRC) -o $(MEMORY_O)

$(IO_ASM_O): $(IO_ASM_SRC)
	nasm -f elf -g $(IO_ASM_SRC) -o $(IO_ASM_O)
##

clean:
	rm -rf $(BOOT_BIN)
	rm -rf $(KERNEL_BIN)
	rm -rf $(OS_BIN)
	rm -rf $(KERNEL_FULL_O)
	rm -rf $(FILES)

