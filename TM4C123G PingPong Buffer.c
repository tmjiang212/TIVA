#include "stdint.h"
#include "stdbool.h"
#include "time.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "utils/ustdlib.h"
#include "Kentec320x240x16_ssd2119_8bit.h"
#include "touch.h"
#include "images.h"
#include "driverlib/adc.h"

#define MEM_BUFFER_SIZE 1024
static uint32_t g_ui32SrcBuf[MEM_BUFFER_SIZE];
static uint32_t g_ui32DstBuf[MEM_BUFFER_SIZE];
static uint32_t g_ui32DMAErrCount = 0;
static uint32_t g_ui32BadISR = 0;
static uint32_t g_ui32MemXferCount = 0;
#pragma DATA_ALIGN(pui8ControlTable, 1024)
uint8_t pui8ControlTable[1024];


void UART1IntHandler(void)
{
      uint32_t ui32Status;
      uint32_t ui32Mode;
      ui32Status = ROM_UARTIntStatus(UART1_BASE, 1);
      ROM_UARTIntClear(UART1_BASE, ui32Status);
      ui32Mode = ROM_uDMAChannelModeGet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT);
      if(ui32Mode == UDMA_MODE_STOP)
      {
            g_ui32RxPingCount++;
            ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT,
            UDMA_MODE_PINGPONG,
            (void *)(UART1_BASE + UART_O_DR),
            g_pui8RxPing, sizeof(g_pui8RxPing));
      }
      ui32Mode = ROM_uDMAChannelModeGet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT);
      if(ui32Mode == UDMA_MODE_STOP)
      {
            g_ui32RxPongCount++;
            ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT,
            UDMA_MODE_PINGPONG,
            (void *)(UART1_BASE + UART_O_DR),
            g_pui8RxPong, sizeof(g_pui8RxPong));
      }
      if(!ROM_uDMAChannelIsEnabled(UDMA_CHANNEL_UART1TX))
      {
            ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT,
            UDMA_MODE_BASIC, g_pui8TxBuf,
            (void *)(UART1_BASE + UART_O_DR),
            sizeof(g_pui8TxBuf));
            ROM_uDMAChannelEnable(UDMA_CHANNEL_UART1TX);
      }
}
void InitUART1Transfer(void)
{
        uint32_t ui32Idx;
        for(ui32Idx = 0; ui32Idx < UART_TXBUF_SIZE; ui32Idx++)
        {
            g_pui8TxBuf[ui32Idx] = ui32Idx;
        }
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
        ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART1);
        ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
        ROM_UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);
        ROM_UARTEnable(UART1_BASE);
        ROM_UARTDMAEnable(UART1_BASE, UART_DMA_RX | UART_DMA_TX);
        HWREG(UART1_BASE + UART_O_CTL) |= UART_CTL_LBE;
        ROM_IntEnable(INT_UART1);
        ROM_uDMAChannelAttributeDisable(UDMA_CHANNEL_UART1RX, UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);
        ROM_uDMAChannelControlSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_4);
        ROM_uDMAChannelControlSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_4);
        ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_pui8RxPing, sizeof(g_pui8RxPing));
        ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_pui8RxPong, sizeof(g_pui8RxPong));
        ROM_uDMAChannelAttributeDisable(UDMA_CHANNEL_UART1TX, UDMA_ATTR_ALTSELECT | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);
        ROM_uDMAChannelAttributeEnable(UDMA_CHANNEL_UART1TX, UDMA_ATTR_USEBURST);
        ROM_uDMAChannelControlSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_4);
        ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC, g_pui8TxBuf, (void *)(UART1_BASE + UART_O_DR), sizeof(g_pui8TxBuf));
        ROM_uDMAChannelEnable(UDMA_CHANNEL_UART1RX);
        ROM_uDMAChannelEnable(UDMA_CHANNEL_UART1TX);
}

int main(void)
{
      volatile uint32_t ui32Loop;
      ROM_FPULazyStackingEnable();
      ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
      ROM_SysCtlPeripheralClockGating(true);
      ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
      ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
      ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
      ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);
      GPIOPinConfigure(GPIO_PA0_U0RX);
      GPIOPinConfigure(GPIO_PA1_U0TX);
      ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
      ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
      ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UDMA);
      ROM_IntEnable(INT_UDMAERR);
      ROM_uDMAEnable();
      ROM_uDMAControlBaseSet(ucControlTable);
      InitUART1Transfer();
      while(1)
      {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            SysCtlDelay(SysCtlClockGet() / 20 / 3);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
            SysCtlDelay(SysCtlClockGet() / 20 / 3);
      }
}
