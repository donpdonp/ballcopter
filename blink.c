#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"
#include "drivers/buttons.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3
 
void delayuS(int);
void timerDelay(unsigned long ulPeriod);
void timerHandler(void);
void timersetup();
void uartsetup();

#define BOTTOM_SPEED_MS 1000
#define TOP_SPEED_MS 2000

int speed = 0;
int sys_clock;

int main() {
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //64mhz
  sys_clock = ROM_SysCtlClockGet();
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, LED_RED|LED_BLUE|LED_GREEN);

  ButtonsInit();
  uartsetup();
  timersetup();
  speed = BOTTOM_SPEED_MS;

  UARTprintf("Throttle at idle (%d uS)\n", speed);

  timerDelay(50); // PWM at 50hz

  unsigned char ucButtons;
  unsigned char ucButtonsChanged;

  for (;;) {
    ucButtons = ButtonsPoll(&ucButtonsChanged, 0);
    if(BUTTON_PRESSED(RIGHT_BUTTON, ucButtons, ucButtonsChanged)) {
      if(speed < TOP_SPEED_MS) {
        speed = speed + 100;
        UARTprintf("More %d uS\n", speed);
      }
    }
    if(BUTTON_PRESSED(LEFT_BUTTON, ucButtons, ucButtonsChanged)) {
      if(speed > BOTTOM_SPEED_MS) {
        speed = speed - 100;
        UARTprintf("Less %d uS\n", speed);
      }
    }
    delayuS(10000);
  }
}

void delayuS(int us) {
  // loop us times, with 3 processor cycles per loop
  ROM_SysCtlDelay( (sys_clock/(3*1000000))*us );
}

void timerDelay(unsigned long hz){
  //unsigned long hz = 1/(microsec/1000000);
  unsigned long ulPeriod = sys_clock/(2*hz);
  TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod-1);
  IntEnable(INT_TIMER0A);
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  IntMasterEnable();
  TimerEnable(TIMER0_BASE, TIMER_A);
}

void timerHandler(void){
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

  // set the B1 pin high, others low
  ROM_GPIOPinWrite(GPIO_PORTB_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_RED);
  delayuS(speed);
  ROM_GPIOPinWrite(GPIO_PORTB_BASE, LED_RED|LED_GREEN|LED_BLUE, 0);
}

void uartsetup() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(0);
}

void timersetup(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  
  TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
}