CC   = x86_64-elf-gcc
LD   = $(CC)
AS   = $(CC)
NASM = nasm

LC_FLAGS  =                 \
    -I. -I.. -Wall -Wextra  \
    -DKERNEL_BUILD          \
    -mcmodel=kernel         \
    -std=gnu11              \
    -ffreestanding          \
    -flto                   \
    -fno-pic                \
    -fno-omit-frame-pointer \
    -mno-mmx                \
    -mno-sse                \
    -mno-sse2               \
    -mno-red-zone           \
    -Wno-cast-function-type \
    -O2 -g

AS_FLAGS  = -I . -flto
ASM_FLAGS = -f elf64 -I.

LD_FLAGS  =                 \
    -flto                   \
    -std=gnu11              \
    -Wno-lto-type-mismatch  \
    -no-pie                 \
    -nostdlib               \
    -Tlinker.ld             \
    -z max-page-size=0x2000 \
    -I .                    \
    -O2 -g

KERNELOBJ = hanos.elf
BOOT_DIR  = 3rd-party/boot

C_FILES   = $(shell find ./ -type f,l -name '*.c')
C_OBJS    = $(C_FILES:.c=.o)
C_DEPS    = $(C_FILES:.c=.d)

LIBC_FILES= $(shell find ../libc -type f,l -name '*.c')
LIBC_OBJS = $(patsubst %.c, %.k.o, ${LIBC_FILES})
LIBC_DEPS = $(patsubst %.c, %.k.d, ${LIBC_FILES})

AS_FILES  = $(shell find ./ -type f,l -name '*.s')
AS_OBJS   = $(AS_FILES:.s=.o)

MAC_FILES = $(shell find ./ -type f,l -name '*.mac')

ASM_FILES = $(shell find ./ -type f,l -name '*.asm')
ASM_OBJS  = $(ASM_FILES:.asm=.o)

OBJS      = $(C_OBJS) $(LIBC_OBJS) $(AS_OBJS) $(ASM_OBJS)

SYMBOLFILE     = _symbols.gen
SYMBOLOBJ      = _symbols.o

TRAMPOLINEFILE = sys/trampoline.nasm
TRAMPOLINEBLOB = sys/trampoline.bin

.PHONY: all clean

all: $(KERNELOBJ) $(SYMBOLFILE)

$(SYMBOLFILE): $(OBJS)
	@echo ">>>>> Generating symbols..."
	@./gensym $@
	$(CC) $(LC_FLAGS) -x c -c $(SYMBOLFILE) -o $(SYMBOLOBJ)

	@echo ">>>>> Linking symbols..."
	$(LD) $(LD_FLAGS) $(OBJS) $(SYMBOLOBJ) -o $(KERNELOBJ)

	@echo ">>>>> Re-generating symbols..."
	@./gensym $(KERNELOBJ)
	$(CC) $(LC_FLAGS) -x c -c $(SYMBOLFILE) -o $(SYMBOLOBJ)

	@echo ">>>>> Re-linking symbols..."
	$(LD) $(LD_FLAGS) $(OBJS) $(SYMBOLOBJ) -o $(KERNELOBJ)

	# Below are used to generate symbol file for gdb debug
	# objcopy --only-keep-debug $(KERNELOBJ) hanos.sym
	# Remove debug information from target executable file
	objcopy --strip-debug $(KERNELOBJ)

$(KERNELOBJ): $(TRAMPOLINEBLOB) $(OBJS)
	$(LD) $(LD_FLAGS) $(OBJS) -o $@

$(BOOT_DIR)/limine.h:
	$(shell [ -d "$(BOOT_DIR)" ] || mkdir -p $(BOOT_DIR))
	@if [ ! -e "$(BOOT_DIR)/limine.h" ]; then wget -P $(BOOT_DIR) https://raw.githubusercontent.com/limine-bootloader/limine/trunk/limine.h; fi

-include $(C_DEPS)

$(C_OBJS): %.o: %.c $(BOOT_DIR)/limine.h
	@echo Compiling $< to $@ ...
	$(CC) -MM -I. -MT $@ -MF $(patsubst %.o, %.d, $@) $<
	$(CC) $(LC_FLAGS) -c $< -o $@

-include $(LIBC_DEPS)

$(LIBC_OBJS): %.k.o: %.c
	@echo Compiling $< to $@ ...
	$(CC) -MM -I. -MT $@ -MF $(patsubst %.k.o, %.k.d, $@) $<
	$(CC) $(LC_FLAGS) -c $< -o $@

$(AS_OBJS): %.o: %.s
	@echo Compiling $< to $@ ...
	@$(AS) -o $@ -c $< $(AS_FLAGS)

$(ASM_OBJS): %.o: %.asm $(MAC_FILES)
	@echo Compiling $< to $@ ...
	@$(NASM) $(ASM_FLAGS) $< -o $@

# build the trampoline blob
$(TRAMPOLINEBLOB): $(TRAMPOLINEFILE)
	@echo Compiling trampoline file... $^
	@$(NASM) $^ -o $@

clean:
	rm -rf $(KERNELOBJ) $(OBJS) $(C_DEPS) $(LIBC_DEPS) $(SYMBOLFILE) $(SYMBOLOBJ) $(TRAMPOLINEBLOB) hanos.sym
