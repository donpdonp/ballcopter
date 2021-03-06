#  wget https://raw.github.com/yuvadm/stellaris/master/boards/ek-lm4f120xl/blinky/startup_gcc.c
#  wget https://raw.github.com/yuvadm/stellaris/master/boards/ek-lm4f120xl/blinky/blinky.ld

#CC=arm-linux-gnueabi-gcc-4.7 
CC=arm-none-eabi-gcc

#LD=arm-linux-gnueabi-ld
LD=arm-none-eabi-ld

all:
	$(CC) blink.c startup_gcc.c uartstdio.c drivers/buttons.c -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall -pedantic -DPART_LM4F120H5QR -c -I../yuvadm-stellaris  -DTARGET_IS_BLIZZARD_RA1 -I.

	$(LD) -T blink.ld --entry ResetISR -o a.out startup_gcc.o blink.o uartstdio.o buttons.o --gc-sections ../yuvadm-stellaris/driverlib/gcc-cm4f/gpio.o -L ../yuvadm-stellaris/driverlib/gcc-cm4f -ldriver-cm4f
	arm-linux-gnueabi-objcopy -O binary a.out blink.bin
