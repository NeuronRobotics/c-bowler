#ifndef __UART_C
#define __UART_C
#include "Bowler/Bowler.h"

void Write32UART2(BYTE data)
{
//	while( (U2STA & 0x00000200) == 1);// Wait for there to be room in the FIFO
//	while( (U2STA & 0x00000100) == 0);// Wait for shift register to empty
//    U2TXREG = data;
	//writePic32Uart(UART2, data);
	//while (!UARTTransmissionHasCompleted(UART2));
	while (!UARTTransmitterIsReady(UART2));
	UARTSendDataByte(UART2, data);
}


void Write32UART1(BYTE  data)
{
//	while( (U1STA & 0x00000200) == 1);// Wait for there to be room in the FIFO
//	while( (U1STA & 0x00000100) == 0);// Wait for shift register to empty
//    U1TXREG = data;
	//while (!UARTTransmissionHasCompleted(UART1));
	while (!UARTTransmitterIsReady(UART1));
	UARTSendDataByte(UART1, data);
}






#endif	//STACK_USE_UART

