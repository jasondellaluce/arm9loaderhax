export PATH	:=	$(DEVKITARM)/bin:$(PATH)

CC=arm-none-eabi-gcc
CP=arm-none-eabi-g++
OC=arm-none-eabi-objcopy
LD=arm-none-eabi-ld
MV=mv -f
RM=rm -rf

LIBNAME=screen_init
ELFNAME=$(LIBNAME).elf
BINNAME=$(LIBNAME).bin
DATNAME=$(LIBNAME).dat

SRC_DIR:=source/$(LIBNAME)
OBJ_DIR:=obj/$(LIBNAME)
LIB_DIR:=lib
DEP_DIR:=obj/$(LIBNAME)

LIBS=
CFLAGS=-std=gnu99 -Os -g -mword-relocations -fomit-frame-pointer -ffast-math
C9FLAGS=-mcpu=arm946e-s -march=armv5te -mlittle-endian
C11FLAGS=-mcpu=mpcore -mlittle-endian
LDFLAGS=$(LIBS)
OCFLAGS=--set-section-flags .bss=alloc,load,contents

OBJS:=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))
OBJS+=$(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.s))
OBJS+=$(patsubst $(SRC_DIR)/%.S, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.S))

OUT_DIR= obj/$(LIBNAME)

.PHONY: clean

all: screen_init.bin

screen_init.bin: $(OBJS)
	$(CC) -nostartfiles --specs=$(LIBNAME).specs $(OBJS) $(LDFLAGS) -o $(ELFNAME)
	$(OC) $(OCFLAGS) -O binary $(ELFNAME) $(BINNAME)

obj/%.o: source/%.c | dirs
	@echo Compiling $<
	$(CC) -c $(CFLAGS) $(C11FLAGS) $< -o $@

obj/%.o: source/%.s | dirs
	@echo Compiling $<
	$(CC) -c $(CFLAGS) $(C11FLAGS) $< -o $@

obj/%.o: source/%.S | dirs
	@echo Compiling $<
	$(CC) -c $(CFLAGS) $(C11FLAGS) $< -o $@

dirs: ${OUT_DIR}

${OUT_DIR}:
	mkdir -p ${OUT_DIR}

clean:
	rm -rf *.elf *.bin obj/*
