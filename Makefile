TOOLCHAIN = riscv32-unknown-elf-

CC = $(TOOLCHAIN)gcc
LD = $(TOOLCHAIN)ld
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump

CFLAGS = -Wall -nostdlib -O3 -mabi=ilp32 -march=rv32imzicsr -fno-builtin -Iinclude

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,%.o,$(SOURCES))

LINKER = dtekv-script.lds

.PHONY: build clean

build: main.bin

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

boot.o: boot.S
	$(CC) $(CFLAGS) -c $< -o $@

main.elf: $(OBJECTS) boot.o $(LINKER)
	$(LD) -o $@ -T $(LINKER) $(OBJECTS) softfloat.a

main.bin: main.elf
	$(OBJCOPY) --output-target binary $< $@
	$(OBJDUMP) -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt
