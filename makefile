# Makefile
all: diskimage bootloader new_shell ls mkdir cat cd mv cp stdlib kernel test

# Recipes
diskimage: bootloader kernel test ls mkdir cat cd mv cp
	# TODO : Tambahkan untuk pembuatan image
	dd if=/dev/zero of=out/system.img bs=512 count=2880
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1
	cd out ; ./tc_gen S ; cd ..

bootloader:
	# TODO : Tambahkan untuk pembuatan bootloader
	nasm src/asm/bootloader.asm -o out/bootloader

kernel: new_shell stdlib
# TODO : Tambahkan untuk pembuatan kernel
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/lib_interrupt.o out/std_lib.o out/fileio.o out/string.o out/textio.o

stdlib:
	# Opsional
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c

new_shell:
	bcc -ansi -c -o out/user_shell.o src/c/user_program/shell.c
	bcc -ansi -c -o out/textio.o src/c/lib/textio.c
	bcc -ansi -c -o out/string.o src/c/lib/string.c
	bcc -ansi -c -o out/sectorio.o src/c/lib/sectorio.c
	bcc -ansi -c -o out/utils.o src/c/lib/utils.c
	bcc -ansi -c -o out/program.o src/c/lib/program.c
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
	bcc -ansi -c -o out/fileio.o src/c/lib/fileio.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/utils_asm.o
	ld86 -o out/shell -d out/user_shell.o out/lib_interrupt.o out/textio.o out/string.o out/sectorio.o out/utils.o out/utils_asm.o out/program.o out/std_lib.o out/fileio.o

ls: new_shell
	bcc -ansi -c -o out/ls.o src/c/user_program/ls.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/utils_asm.o
	ld86 -o out/ls -d out/ls.o out/textio.o out/utils.o out/program.o out/std_lib.o out/lib_interrupt.o out/utils_asm.o out/sectorio.o out/string.o out/fileio.o

mkdir: new_shell
	bcc -ansi -c -o out/mkdir.o src/c/user_program/mkdir.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/utils_asm.o
	ld86 -o out/mkdir -d out/mkdir.o out/textio.o out/utils.o out/program.o out/std_lib.o out/lib_interrupt.o out/utils_asm.o out/sectorio.o out/string.o out/fileio.o

cat: new_shell
	bcc -ansi -c -o out/cat.o src/c/user_program/cat.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/utils_asm.o
	ld86 -o out/cat -d out/cat.o out/textio.o out/utils.o out/program.o out/std_lib.o out/lib_interrupt.o out/utils_asm.o out/sectorio.o out/string.o out/fileio.o

cd: new_shell
	bcc -ansi -c -o out/cd.o src/c/user_program/cd.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/utils_asm.o
	ld86 -o out/cd -d out/cd.o out/textio.o out/utils.o out/program.o out/std_lib.o out/lib_interrupt.o out/utils_asm.o out/sectorio.o out/string.o out/fileio.o

mv: new_shell
	bcc -ansi -c -o out/mv.o src/c/user_program/mv.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/utils_asm.o
	ld86 -o out/mv -d out/mv.o out/textio.o out/utils.o out/program.o out/std_lib.o out/lib_interrupt.o out/utils_asm.o out/sectorio.o out/string.o out/fileio.o

cp: new_shell
	bcc -ansi -c -o out/cp.o src/c/user_program/cp.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/utils_asm.o
	ld86 -o out/cp -d out/cp.o out/textio.o out/utils.o out/program.o out/std_lib.o out/lib_interrupt.o out/utils_asm.o out/sectorio.o out/string.o out/fileio.o

run:
	echo "c" | bochs -f src/config/if2230.config

build-run: all run

test:
	gcc -o out/tc_gen tc_gen/tc_gen.c tc_gen/tc_lib
	
