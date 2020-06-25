# the name of the program we're going to build
PROJECT = program
# build directory
BUILD = build

# source files
C_FILES := $(wildcard otter/*.c)
AS_FILES := $(wildcard otter/*.s)
ASP_FILES := $(wildcard otter/*.S)

# object files
C_OBJ_FILES := $(patsubst src/%.c,build/%.o,$(C_FILES))
AS_OBJ_FILES := $(patsubst src/%.s,build/%.o,$(AS_FILES))
ASP_OBJ_FILES := $(patsubst src/%.S,build/%.o,$(ASP_FILES))

# settings for the compilers
ifneq ($(C_FILES),)
RISCV_PREFIX = riscv32-unknown-elf-
else
RISCV_PREFIX = riscv64-unknown-elf-
endif
CC = $(RISCV_PREFIX)gcc
LD = $(RISCV_PREFIX)ld
OBJCOPY = $(RISCV_PREFIX)objcopy
OBJDUMP = $(RISCV_PREFIX)objdump

TARGET_ARCH =  -march=rv32i -mabi=ilp32
ifneq ($(C_FILES),)
CCFLAGS = -O3 $(TARGET_ARCH) -g
else
CCFLAGS = -O0 $(TARGET_ARCH)
endif
#-fno-builtin
#-mstrict-align
#-static -static-libgcc -nostdlib
#-S   #flag for getting assembly output directly

# Lists of rules: the name of a file, followed by the files it depends on.
# When typing 'make' not followed by a rule name, the first rule is followed.  This rule depends on
# everything, causing all the parts to be built

all: dirs $(BUILD)/mem.txt $(BUILD)/$(PROJECT).dump

# how to build a .o file from a .c file
$(BUILD)/%.o: src/%.c
	$(CC) -c -o $@ $< $(CCFLAGS) 

# how to build a .o file from a .s file
$(BUILD)/%.o: src/%.s
	$(CC) -c -o $@ $< $(CCFLAGS)

# how to build a .o file from a .S file
$(BUILD)/%.o: src/%.S
	$(CC) -c -o $@ $< $(CCFLAGS)

# link with gcc
$(BUILD)/$(PROJECT).elf: ${AS_OBJ_FILES} ${C_OBJ_FILES} ${ASP_OBJ_FILES}
ifneq ($(C_FILES),)
	$(CC)  -o $@ $^ -T link.ld $(CCFLAGS) 
else
	$(CC) -o $@ $^ -T link.ld -mno-relax -nostdlib -nostartfiles -mcmodel=medany $(CCFLAGS)
endif

#-Wl  --no-relax

$(BUILD)/$(PROJECT).dump: $(BUILD)/$(PROJECT).elf
ifneq ($(C_FILES),)
	$(OBJDUMP) --no-show-raw-insn -S -s  $< > $@
else
	$(OBJDUMP) -S -s $< > $@
endif
#no-show-raw_insn
#-l line numbers

# extract the binary data from data and text sections to get a binary image of memory
$(BUILD)/mem.bin: $(BUILD)/$(PROJECT).elf
	$(OBJCOPY) -O binary --only-section=.data* --only-section=.text* $< $@

# convert to an ASCII hex file for OTTER memory
$(BUILD)/mem.txt: $(BUILD)/mem.bin
	hexdump -v -e '"%08x\n"' $< > $@

# make software project folder
dirs:
	mkdir -p $(BUILD)

# 'clean' rule: delete all the files so we can start afresh
clean:
	rm -rf $(BUILD)

print-%  : ; @echo $* = $($*)
