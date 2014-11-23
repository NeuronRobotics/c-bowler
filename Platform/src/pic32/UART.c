#ifndef __UART_C
#define __UART_C
#include "arch/pic32/BowlerConfig.h"
#include "Bowler/Bowler.h"

boolean useUart2 = false; 
boolean useUart1 = false; 

#define delayUart 5000

boolean Write32UART2(uint8_t data)
{
	int tick =delayUart;
	while (BusyUART2()){

		if(tick--==0){
			return false; 
		}
	}
	WriteUART2(data);
	//UARTSendDataByte(UART2, data);
	return true; 
}


boolean Write32UART1(uint8_t  data)
{

    int tick =delayUart;
	while (!UARTTransmitterIsReady(UART1)){
            if(INTGetFlag(INT_SOURCE_UART_ERROR(UART1))){
		INTClearFlag(INT_SOURCE_UART_ERROR(UART1));
            }
            if(tick--==0){
                return false; 
            }
	}
	UARTSendDataByte(UART1, data);
	return true; 
}

#endif	//STACK_USE_UART

