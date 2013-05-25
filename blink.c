#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
 
#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3
 
int main() {
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, LED_RED|LED_BLUE|LED_GREEN);
  int speed = 1000;
  for (;;) {
    // set the red LED pin high, others low
    ROM_GPIOPinWrite(GPIO_PORTB_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_RED);
    delayuS(speed);
    ROM_GPIOPinWrite(GPIO_PORTB_BASE, LED_RED|LED_GREEN|LED_BLUE, 0);
    delayuS(2000-speed);
  }
}

void delayuS(int us) {
    ROM_SysCtlDelay( (ROM_SysCtlClockGet()/(3*1000000))*us ) ;  // more accurate
}

