#  wget https://raw.github.com/yuvadm/stellaris/master/boards/ek-lm4f120xl/blinky/startup_gcc.c
#  wget https://raw.github.com/yuvadm/stellaris/master/boards/ek-lm4f120xl/blinky/blinky.ld

CC=arm-linux-gnueabi-gcc-4.7 

#LD=arm-none-eabi-ld
LD=arm-linux-gnueabi-ld

all:
	$(CC) blink.c startup_gcc.c -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall -pedantic -DPART_LM4F120H5QR -c -I../yuvadm-stellaris  -DTARGET_IS_BLIZZARD_RA1
	$(CC) blink.c uartstdio.c -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall -pedantic -DPART_LM4F120H5QR -c -I../yuvadm-stellaris  -DTARGET_IS_BLIZZARD_RA1
	$(LD) -T blink.ld --entry ResetISR -o a.out startup_gcc.o blink.o uartstdio.o --gc-sections ../yuvadm-stellaris/driverlib/gcc-cm4f/gpio.o
	arm-linux-gnueabi-objcopy -O binary a.out blink.bin
