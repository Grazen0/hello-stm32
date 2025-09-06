TARGET := hello.elf
BOARD := STM32F410Rx

BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := ./src \
			./external/CMSIS/Device/ST/STM32F4/Include \
			./external/CMSIS/CMSIS/Core/Include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC := arm-none-eabi-gcc
CPPFLAGS := $(INC_FLAGS) -MMD -MP
CFLAGS := -mcpu=cortex-m4 -mthumb -nostdlib -std=c23 -D$(BOARD)
LDFLAGS := -T ./data/linker_script.ld

PROGRAMMER := openocd
PROGRAMMER_FLAGS := -f interface/stlink.cfg -f target/stm32f4x.cfg

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET)
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program $(BUILD_DIR)/$(TARGET) verify reset exit"

.PHONY: clean all flash

-include $(DEPS)
