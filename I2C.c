#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c129xnczad.h"

#define SLAVE_ADDRESS 0x3C

void delay(void)
{
    volatile uint32_t ui32Loop;   
    for(ui32Loop = 0; ui32Loop < 200; ui32Loop++);
}


volatile uint32_t  result;
uint32_t    g_ui32SysClock;

int main(void)
{
    //Set Clock
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_OSC_MAIN | SYSCTL_XTAL_25MHZ | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320), 40000000); 

    // Enable the GPIO 
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R3 | SYSCTL_RCGCGPIO_R9 | SYSCTL_RCGCGPIO_R1;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    result = SYSCTL_RCGCGPIO_R;

    // Enable the GPIO pin for the LED (PD3).  Set the direction as output, and enable the GPIO pin for digital function.
    GPIO_PORTD_AHB_DIR_R = 0x8;
    GPIO_PORTD_AHB_DEN_R = 0x8;

    SYSCTL_RCGCI2C_R = (1 << 4) | (1 << 6); // Mode Clock Gating Control for I2C modules 4 and 6

    GPIOPinConfigure(GPIO_PK6_I2C4SCL);
    GPIOPinConfigure(GPIO_PK7_I2C4SDA);
    GPIOPinConfigure(GPIO_PB6_I2C6SCL);
    GPIOPinConfigure(GPIO_PB7_I2C6SDA);

    GPIOPinTypeI2C(GPIO_PORTK_BASE, (1 << 7));       // Configures SDA
    GPIOPinTypeI2CSCL(GPIO_PORTK_BASE, (1 << 6));    // Configures SCL
    GPIOPinTypeI2C(GPIO_PORTB_BASE, (1 << 7));       // Configures SDA
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, (1 << 6));    // Configures SCL

    //Set I2C Master Clock
    I2CMasterInitExpClk(I2C4_BASE, g_ui32SysClock, false);
    //Config slave
    I2CSlaveEnable(I2C6_BASE);
    I2CSlaveInit(I2C6_BASE, SLAVE_ADDRESS);
    I2CMasterSlaveAddrSet(I2C4_BASE, SLAVE_ADDRESS, false);

    // Loop forever.
    while(1)
    {

        // Turn on the LED.
        GPIO_PORTD_AHB_DATA_R |= 0x8;

        I2CMasterDataPut(I2C4_BASE, 0x33);
        I2CMasterControl(I2C4_BASE, I2C_MASTER_CMD_SINGLE_SEND);
      
        // Wait until the slave has received and acknowledged the data.
        while(!(I2CSlaveStatus(I2C6_BASE) & I2C_SLAVE_ACT_RREQ));           
        // Read the data from the slave.
        result = I2CSlaveDataGet(I2C6_BASE);

        // Wait until master module is done transferring.
        while(I2CMasterBusy(I2C4_BASE));
        delay   ();

        // Turn off the LED.
        GPIO_PORTD_AHB_DATA_R &= ~(0x8);

        delay();
    }
}
