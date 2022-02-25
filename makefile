# Makefile
all: diskimage bootloader stdlib kernel

# Recipes
diskimage: bootloader kernel
	# TODO : Tambahkan untuk pembuatan image
	dd if=/dev/zero of=out/system.img bs=512 count=2880
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

bootloader:
	# TODO : Tambahkan untuk pembuatan bootloader
	nasm src/asm/bootloader.asm -o out/bootloader

kernel: stdlib
	# TODO : Tambahkan untuk pembuatan kernel
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o

stdlib:
	# Opsional
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c

run:
	bochs -f src/config/if2230.config

build-run: all run

