KERNEL := kernel
KERNEL_DIR := .
BUILD = build
RAMDISK = userland

OPTI = -Ofast

CC = gcc
CFLAGS = -O0 -g -m32 -Wall -Wextra $(OPTI) -I$(KERNEL_DIR) -std=gnu17

ASMPARAM = -f elf32 -F dwarf

CORE_NUM = 1

LINK_SCRIPT = $(KERNEL_DIR)/link.ld

INTERNALLDFLAGS :=     \
	-nostdlib      \
	-T$(LINK_SCRIPT)    \
	-z max-page-size=0x1000\
	-m elf_i386	\

INTERNALCFLAGS  :=                  \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic -fno-pie    \
	-mno-sse			 \
	-fno-zero-initialized-in-bss \
	-fno-isolate-erroneous-paths-attribute \
	-fno-delete-null-pointer-checks

SRCDIRS += $(shell find ./$(KERNEL_DIR) -type d)

CFILES :=  $(shell find ./$(KERNEL_DIR) -type f -name '*.c')
SFILES :=  $(shell find ./$(KERNEL_DIR) -type f -name '*.s')
GASFILES := $(shell find ./$(KERNEL_DIR) -type f -name '*.S')

OBJ    := $(GASFILES:%.S=$(BUILD)/%.o)
OBJ    += $(SFILES:%.s=$(BUILD)/%.o)
OBJ    += $(CFILES:%.c=$(BUILD)/%.o)

QEMU = qemu-system-i386
QEMU_PARAMS_NODEBUG = -no-reboot -vga std -D ./log.txt -d int,guest_errors -m 512M -boot d -M q35  -serial mon:stdio -m 1G -smp $(CORE_NUM) -cdrom
QEMU_PARAMS_DEBUG = -S -gdb tcp::9000

$(shell mkdir -p $(addprefix $(BUILD)/,$(SRCDIRS)))

all: NODEBUG

.PHONY: all clean NODEBUG DEBUG clean_deps ramdisk

NODEBUG: $(KERNEL).iso
	@$(QEMU) $(QEMU_PARAMS_NODEBUG) $<

DEBUG: $(KERNEL).iso
	@$(QEMU) $(QEMU_PARAMS_DEBUG) $(QEMU_PARAMS_NODEBUG) $<

$(KERNEL).iso: $(KERNEL).elf
	mkdir -p iso
	mkdir -p iso/boot
	mkdir -p iso/boot/grub
	touch iso/boot/grub/grub.cfg
	cp $< iso/boot/$<
	@echo 'menuentry "Raccoon_kernel" {'  			>> iso/boot/grub/grub.cfg
	@echo '  multiboot /boot/$<'          		>> iso/boot/grub/grub.cfg
	@echo '}'                                  	>> iso/boot/grub/grub.cfg
	@echo "---------------------------"
	@echo "+ BEGINNING OF XORRISO LOG:"
	@echo "---------------------------"
	grub-mkrescue /usr/lib/grub/i386-pc -o $@ iso
	@echo "---------------------------"
	@echo "+ END OF XORRISO LOG       "
	@echo "---------------------------"
	rm -rf iso


$(KERNEL).elf: $(OBJ)
	ld $(INTERNALLDFLAGS) $(OBJ) -o $@

$(BUILD)/%.o: %.c
	$(CC) -o $@ $(CFLAGS) $(INTERNALCFLAGS) -c $<

$(BUILD)/%.o: %.s
	nasm $(ASMPARAM) -o $@ $<

$(BUILD)/%.o: %.S
	as --32 -o $@ -c $<


# ramdisk:
# 	@make -C $(RAMDISK)

clean:
	rm -rf $(KERNEL).elf $(OBJ) $(KERNEL).iso $(BUILD) log.txt iso_root

