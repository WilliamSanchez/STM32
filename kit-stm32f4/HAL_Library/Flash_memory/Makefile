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

STMSRCDIR = $(STMDIR)/STM32F4xx_HAL_Driver/Src
vpath %.c $(STMSRCDIR)

INC_DIRS = $(STMDIR)/STM32F4xx_HAL_Driver/Inc
INC_DIRS += $(STMDIR)/CMSIS/Core/Include
INC_DIRS += $(STMDIR)/CMSIS/Device/ST/STM32F4xx/Include
INC_DIRS += $(STMDIR)/CMSIS/Include
INC_DIRS += .

INCLUDE = $(addprefix -I,$(INC_DIRS))

###################################################

CFLAGS  = -std=gnu99 -g -O2 -Wall -TSTM32F411VETx_FLASH.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -DUSE_HAL_DRIVER -DSTM32F411xE
CFLAGS += -specs=nosys.specs

LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(TARGET).map

###################################################


SRCS =  main.c stm32f4xx_it.c \
        system_stm32f4xx.c \
       	stm32f4xx_hal_cortex.c \
        startup_stm32f411xe.s 
 

SRCS += stm32f4xx_hal_rcc.c \
	stm32f4xx_hal_rcc_ex.c \
	stm32f4xx_hal_gpio.c \
        stm32f4xx_hal_exti.c \
        stm32f4xx_hal_flash.c \
        stm32f4xx_hal_flash_ex.c \
	stm32f4xx_hal.c


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
	#$(SF) write $(TARGET).bin 0x8000000
	stm32flash -w $(TARGET).bin -v -g 0x0 /dev/ttyUSB0
