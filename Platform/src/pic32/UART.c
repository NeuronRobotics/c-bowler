#ifndef __UART_C
#define __UART_C
#include "arch/pic32/BowlerConfig.h"
#include "Bowler/Bowler.h"

boolean useUart2 = false;
boolean useUart1 = false;

#define delayUart 50000

boolean Write32UART2(uint8_t data) {
    UART2ClearAllErrors();
    int tick = delayUart;
    while (!UARTTransmitterIsReady(UART2)) {

        if (tick-- < 0) {
            return false;
        }
    }
    UARTSendDataByte(UART2, data);
    while (!UARTTransmissionHasCompleted(UART2)) {

        if (tick-- < 0) {
            return false;
        }
    }
    return true;
}

boolean Write32UART1(uint8_t data) {
    UART1ClearAllErrors();
    int tick = delayUart;
    while (!UARTTransmitterIsReady(UART1)) {

        if (tick-- < 0) {
            return false;
        }
    }
    UARTSendDataByte(UART1, data);
    while (!UARTTransmissionHasCompleted(UART1)) {

        if (tick-- < 0) {
            return false;
        }
    }
    return true;
}

#endif	//STACK_USE_UART

