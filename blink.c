#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
 
#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3
 
void delayuS(int);
void uartsetup();
 
int main() {
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //64mhz
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, LED_RED|LED_BLUE|LED_GREEN);

  uartsetup();
  int speed = 1100;

  UARTprintf("Spinning at %d uS\n", speed);

  for (;;) {
    // set the red LED pin high, others low
    ROM_GPIOPinWrite(GPIO_PORTB_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_RED);
    delayuS(speed);
    ROM_GPIOPinWrite(GPIO_PORTB_BASE, LED_RED|LED_GREEN|LED_BLUE, 0);
    delayuS(2000-speed);
  }
}

void delayuS(int us) {
  // loop us times, with 3 processor cycles per loop
  ROM_SysCtlDelay( (ROM_SysCtlClockGet()/(3*1000000))*us );
}

void uartsetup() {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(0);
}

