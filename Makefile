BOOT_SRC = ./src/boot/boot.asm
KERNEL_SRC = ./src/kernel.asm

BOOT_BIN = ./bin/boot.bin
KERNEL_O = ./build/kernel.asm.o
KERNEL_BIN = ./bin/kernel.bin

KERNEL_FULL_O = ./build/kernelfull.o

OS_BIN = ./bin/os.bin

FILES = $(KERNEL_O)

all: $(BOOT_BIN) $(KERNEL_BIN)
	rm -rf $(OS_BIN)
	dd if=$(BOOT_BIN) >> ./bin/os.bin

$(KERNEL_BIN): $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o $(KERNEL_FULL_O)
	i686-elf-gcc -T ./src/linker.ld -o $(KERNEL_BIN) -ffreestanding -O0 -nostdlib $(KERNEL_FULL_O)

$(BOOT_BIN): $(BOOT_SRC)
	nasm -f bin $(BOOT_SRC) -o $(BOOT_BIN)

$(KERNEL_O): $(KERNEL_SRC)
	nasm -f elf -g $(KERNEL_SRC) -o $(KERNEL_O) 

clean:
	rm -rf $(BOOT_BIN)

