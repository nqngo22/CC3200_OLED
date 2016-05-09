//Nguyen Ngo
//Surinder Singh

// Standard includes
#include <string.h>
#include <stdint.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "spi.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "uart.h"
#include "interrupt.h"

// Common interface includes
#include "uart_if.h"
#include "pin_mux_config.h"
#include "gpio.h"
#include "gpio_if.h"

// Includes Adafruit
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"
#include "glcdfont.h"

// Includes test
#include "test.h"

#define APPLICATION_VERSION     "1.1.1"

//*****************************************************************************
//
// Application Master/Slave mode selector macro
//
// MASTER_MODE = 1 : Application in master mode
// MASTER_MODE = 0 : Application in slave mode
//
//*****************************************************************************
#define MASTER_MODE      1

#define SPI_IF_BIT_RATE  100000
#define TR_BUFF_SIZE     100

#define MASTER_MSG       "This is CC3200 SPI Master Application\n\r"
#define SLAVE_MSG        "This is CC3200 SPI Slave Application\n\r"

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
//static unsigned char g_ucTxBuff[TR_BUFF_SIZE];
//static unsigned char g_ucRxBuff[TR_BUFF_SIZE];
//static unsigned char ucTxBuffNdx;
//static unsigned char ucRxBuffNdx;

#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
  //
  // Set vector table base
  //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

//*****************************************************************************
//
//! Main function for spi demo application
//!
//! \param none
//!
//! \return None.
//
//*****************************************************************************
void main()
{
	//Initilize launchpad
    BoardInit();

    // Muxing UART and SPI lines. Uses GPIO pin 18 and 53
    PinMuxConfig();

    // Reset SPI
	MAP_SPIReset(GSPI_BASE);

	//Enables the transmit and/or receive FIFOs.
	//Base address is GSPI_BASE, SPI_TX_FIFO || SPI_RX_FIFO are the FIFOs to be enabled
	MAP_SPIFIFOEnable(GSPI_BASE, SPI_TX_FIFO || SPI_RX_FIFO);

    // Configure SPI interface
    MAP_SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),
                     SPI_IF_BIT_RATE,SPI_MODE_MASTER,SPI_SUB_MODE_0,
                     (SPI_SW_CTRL_CS |
                     SPI_4PIN_MODE |
                     SPI_TURBO_OFF |
                     SPI_CS_ACTIVELOW |
                     SPI_WL_8));

    // Enable SPI for communication
    MAP_SPIEnable(GSPI_BASE);

    // Initialize Adafruit
    Adafruit_Init();

    // Initialising the Terminal.
    InitTerm();
    // Clearing the Terminal.
    ClearTerm();

    // Display the Banner
    Message("\n\n\n\r");
    Message("\t\t   ********************************************\n\r");
    Message("\t\t        CC3200 SPI OLED Application  \n\r");
    Message("\t\t   ********************************************\n\r");
    Message("\n\n\n\r");

    while(1){
    	//OLED drawing functions are in test.c
    	//MAP_UtilsDelay(1600000) is a 0.1 sec delay after each functions so that the patterns can be easily seen
		drawAllFont();
		MAP_UtilsDelay(1600000);
		printHelloWorld();
		MAP_UtilsDelay(1600000);
		drawHorizontalLines();
		MAP_UtilsDelay(1600000);
		drawVerticalLines();
		MAP_UtilsDelay(1600000);
		testlines(BLUE);
		MAP_UtilsDelay(1600000);
		testfastlines(BLUE, RED);
		MAP_UtilsDelay(1600000);
		testdrawrects(WHITE);
		MAP_UtilsDelay(1600000);
		testfillcircles(3, WHITE);
		MAP_UtilsDelay(1600000);
		testdrawcircles(3, CYAN);
		MAP_UtilsDelay(1600000);
		testroundrects();
		MAP_UtilsDelay(1600000);
		testtriangles();
		MAP_UtilsDelay(1600000);
    }
}
