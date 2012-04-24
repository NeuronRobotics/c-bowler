#ifndef __UART_C
#define __UART_C
#include "Bowler/Bowler.h"

BOOL useUart2 = FALSE;
BOOL useUart1 = FALSE;



BOOL Write32UART2(BYTE data)
{
//	while( (U2STA & 0x00000200) == 1);// Wait for there to be room in the FIFO
//	while( (U2STA & 0x00000100) == 0);// Wait for shift register to empty
//    U2TXREG = data;
	//writePic32Uart(UART2, data);
	//while (!UARTTransmissionHasCompleted(UART2));


    if(useUart2==FALSE){
        UARTSetFifoMode(UART2, UART_INTERRUPT_ON_RX_NOT_EMPTY|UART_INTERRUPT_ON_TX_DONE);
        UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_EVEN | UART_STOP_BITS_1);
        UARTSetDataRate(UART2, GetPeripheralClock(), INTERNAL_BAUD );
		UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
        useUart2=TRUE;
    }
	int tick =5000;
	while (!UARTTransmitterIsReady(UART2)){
		if(tick--==0){
			return FALSE;
		}
	}
	UARTSendDataByte(UART2, data);
	return TRUE;
}


BOOL Write32UART1(BYTE  data)
{
//	while( (U1STA & 0x00000200) == 1);// Wait for there to be room in the FIFO
//	while( (U1STA & 0x00000100) == 0);// Wait for shift register to empty
//    U1TXREG = data;
	//while (!UARTTransmissionHasCompleted(UART1));
    if(useUart1==FALSE){
        UARTSetFifoMode(UART1, UART_INTERRUPT_ON_RX_NOT_EMPTY|UART_INTERRUPT_ON_TX_DONE);
		UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
		UARTSetDataRate(UART1, GetPeripheralClock(), PRINT_BAUD );
		UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
        useUart1=TRUE;
    }
	while (!UARTTransmitterIsReady(UART1));
	UARTSendDataByte(UART1, data);
	return TRUE;
}






#endif	//STACK_USE_UART

