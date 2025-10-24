LDSCRIPT = sam/flash.ld
BOOTUP = sam/sam3x/source/exceptions.c sam/sam3x/source/startup_sam3x.c sam/sam3x/source/system_sam3x.c
MCUTYPE = __SAM3X8E__

SOURCE_FILES = \
	$(BOOTUP) \
	main.c \
	drivers/can_controller.c drivers/can_interrupt.c drivers/time.c drivers/uart.c \
	$(wildcard sam/interrupt/*.c)

# NB: For å unngå kollisjon når to filer har samme navn i ulike mapper
BUILD_DIR := build
# Behold hele stien i object-fila i stedet for $(notdir ...)
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCE_FILES))

# Tools
CC:=arm-none-eabi-gcc
LD:=arm-none-eabi-gcc
AR:=arm-none-eabi-ar
AS:=arm-none-eabi-as

ELF=$(BUILD_DIR)/main.elf

LDFLAGS:= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -Wl,--gc-sections -Wl,-Map,$(BUILD_DIR)/main.map
CFLAGS:= -mcpu=cortex-m3 -mthumb -g -O0 -std=c11 -MMD
CFLAGS+= -I sam -I sam/sam3x/include -I sam/sam3x/source -I sam/cmsis -I drivers -I .
CFLAGS+= -D$(MCUTYPE)

.DEFAULT_GOAL := $(ELF)

# Lag underkataloger i build/ som matcher kildestier
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Regler for å lage undermapper automatisk i build/
$(BUILD_DIR)/%/.dir:
	mkdir -p $(dir $@)
	touch $@

# Compile C -> obj (med auto-dir)
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)/%/.dir
	$(CC) -c $(CFLAGS) $< -o $@

# Assemble S -> obj (om du har .s)
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)/%/.dir
	$(AS) $< -o $@

# Link
$(ELF): $(OBJS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: debug
debug: $(ELF)
	# rydd vekk ev. gammel openocd
	pkill -9 openocd || true
	x-terminal-emulator -e openocd -f sam/openocd.cfg -c "program $(ELF) verify"
	sleep 5
	arm-none-eabi-gdb -tui -iex "target extended-remote localhost:3333" $(ELF)
	pkill -9 openocd || true

.PHONY: flash
flash: $(ELF)
	#./sam/write_bin.sh sam/openocd.cfg $(ELF)
	pkill -9 openocd || true
	openocd -f sam/openocd.cfg -c "program $(ELF) verify reset exit"

# pull in dependencies
-include $(OBJS:.o=.d)
