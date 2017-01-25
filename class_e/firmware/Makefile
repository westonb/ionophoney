# Makefile for STM32F051 using cmsis and GNU toolchain.

# Files to include
C_SRC  = $(wildcard src/*.c)
C_SRC += $(wildcard lib/STM32F30x_StdPeriph_Driver/src/*.c)
C_INC  = $(wildcard inc/*.h)

OBJECTS = $(patsubst %.c,%.o,$(C_SRC)) src/startup.o

LIBRARIES = lib/CMSIS/Lib/ARM/arm_cortexM4lf_math.lib

INC  = -Iinc
INC += -Ilib/CMSIS/Include -Ilib/CMSIS/Device/ST/STM32F30x/Include
INC += -Ilib/STM32F30x_StdPeriph_Driver/inc

# Assembler, compiler, and linker flags
AFLAGS  = -mlittle-endian -mthumb -mcpu=cortex-m4
CFLAGS  = $(AFLAGS) -g -O3 $(INC) -std=c99 -DUSE_STDPERIPH_DRIVER -Wall #debugging flag is on this line
#CFLAGS += -DSTM32F031
CFLAGS += -D'HSE_VALUE=((uint32_t)13560000)' #crystal frequency
CFLAGS += -D'STM32F334x8'
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -mfloat-abi=hard -DARM_MATH_CM4 -D'__FPU_PRESENT=1'
CFLAGS += -fdata-sections -ffunction-sections
LFLAGS  = $(CFLAGS) -nostartfiles -Tflash.ld -Wl,-Map=main.map -Wl,--gc-sections

# Targets
all: main.bin
clean:
	-rm -f $(OBJECTS) *.elf *.bin *.map
main.bin: main.elf
	arm-none-eabi-objcopy -O binary main.elf main.bin
main.elf: $(OBJECTS)
	arm-none-eabi-gcc $(LFLAGS) -o main.elf $(OBJECTS) $(LIBRARIES)
src/startup.o: src/startup.s
	arm-none-eabi-as $(AFLAGS) src/startup.s -o src/startup.o
%.o: %.c $(C_INC)
	arm-none-eabi-gcc $(CFLAGS) -c -o $@ $<

%.S: %.c $(C_INC)
	 arm-none-eabi-gcc $(CFLAGS) -S -o $@ $<
gdb: main.elf
	arm-none-eabi-gdb -x init.gdb
load: main.elf
	arm-none-eabi-gdb -x init.gdb -ex "mon reset halt" -ex load
loadquit: main.elf
	arm-none-eabi-gdb -batch -x init.gdb -ex load -ex kill -ex quit
stlink:
	st-util -p 4244
ocd:
	openocd -f board_openocd.cfg
