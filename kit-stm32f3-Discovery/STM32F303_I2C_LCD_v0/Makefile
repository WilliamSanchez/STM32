# -
# Teclado 3G 
# -

TARGET=main
STMDIR=../Libraries

###################################################

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
SE=arm-none-eabi-size
SF=st-flash

###################################################

STMSRCDIR = $(STMDIR)/STM32F30x_StdPeriph_Driver/src
vpath %.c $(STMSRCDIR)

INC_DIRS = $(STMDIR)/STM32F30x_StdPeriph_Driver/inc
INC_DIRS += $(STMDIR)/CMSIS/Device/ST/STM32F30x/Include
INC_DIRS += $(STMDIR)/CMSIS/Include
INC_DIRS += .

INCLUDE = $(addprefix -I,$(INC_DIRS))

###################################################

CFLAGS  = -std=gnu99 -g -O2 -Wall -TSTM32F303VC_FLASH.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -DUSE_STDPERIPH_DRIVER -DSTM32F303xC
CFLAGS += -specs=nosys.specs

LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(TARGET).map

###################################################


SRCS =  main.c stm32f30x_it.c \
        syscalls.c \
	modulo3G.c \
	configI2C.c \
 	misc.c \
        system_stm32f30x.c \
        startup_stm32f30x.s 
 

SRCS += stm32f30x_rcc.c \
	stm32f30x_gpio.c \
	stm32f30x_usart.c \
	stm32f30x_misc.c \
	stm32f30x_i2c.c \
        stm32f30x_exti.c \
        stm32f30x_rtc.c \
	stm32f30x_pwr.c \
	stm32f30x_syscfg.c\
	stm32f30x_comp.c


###################################################

.PHONY: $(TARGET) all clean flash

all: $(TARGET).elf

$(TARGET).elf: $(SRCS) 
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@ 
	$(CP) -O ihex $(TARGET).elf $(TARGET).hex
	$(CP) -O binary $(TARGET).elf $(TARGET).bin


clean:
	rm -f *.o $(TARGET) $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).map *~

flash: 
	$(SF) write $(TARGET).bin 0x8000000
	#stm32flash -w $(TARGET).bin -v -g 0x0 /dev/ttyUSB0

