BOOT_SRC = ./src/boot/boot.asm
KERNEL_SRC = ./src/kernel.asm

BOOT_BIN = ./bin/boot.bin
KERNEL_O = ./build/kernel.asm.o

OS_BIN = ./bin/os.bin

all: $(BOOT_BIN) $(KERNEL_O)
	rm -rf $(OS_BIN)
	dd if=$(BOOT_BIN) >> ./bin/os.bin

$(BOOT_BIN): $(BOOT_SRC)
	nasm -f bin $(BOOT_SRC) -o $(BOOT_BIN)

$(KERNEL_O): $(KERNEL_SRC)
	nasm -f elf -g $(KERNEL_SRC) -o $(KERNEL_O) 

clean:
	rm -rf $(BOOT_BIN)

