SRC_DIR = ./src
BIN_DIR = ./bin
BOOT_SRC = $(SRC_DIR)/boot.asm
BOOT_BIN = $(BIN_DIR)/boot.bin

$(BOOT_BIN): $(BOOT_SRC)
	nasm -f bin $(BOOT_SRC) -o $(BOOT_BIN)
	dd if=$(SRC_DIR)/boot_message.txt >> $(BOOT_BIN)

clean:
	rm -f $(BOOT_BIN)

.PHONY: clean
