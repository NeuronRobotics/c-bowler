/**
 *
 * Copyright 2009 Neuron Robotics, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "arch/pic32/BowlerConfig.h"
#include "Bowler/Bowler.h"

BYTE_FIFO_STORAGE * my_store = NULL;
static boolean UARTinitialized = false;
static int cachedbaud=0;
//BYTE_FIFO_STORAGE * GetPICUARTFifo() {
//    return my_store;
//}

void SetPICUARTFifo(BYTE_FIFO_STORAGE * s) {
    my_store = s;
}

void newByteUartHal() {
    uint8_t err;

//    int timeout = 0;
    INTEnable(INT_SOURCE_UART_RX(UART1), INT_DISABLED);
//    while (U1STAbits.URXDA != 0  && timeout < 5) {
//
        FifoAddByte(my_store, UARTGetDataByte(UART1), &err);
//
//        U1STAbits.URXDA = 0;
//        //buttonCheck(17);
//        timeout++;
//    }
    INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
}

void __ISR(_UART_1_VECTOR, IPL2AUTO) My_U1_ISR(void){


    if (INTGetFlag(INT_SOURCE_UART_RX(UART1))) {
        newByteUartHal();
        //WriteUART_COM(UARTGetDataByte(UART1)ddd);
        INTClearFlag(INT_SOURCE_UART_RX(UART1));
    }
    int err = UART1GetErrors();
    if (err) {
        UART1ClearAllErrors();
        UARTinitialized=false;
        Pic32UARTSetBaud(cachedbaud);

    }
    //EndCritical();
    if (INTGetFlag(INT_SOURCE_UART_ERROR(UART1))) {
                INTClearFlag(INT_SOURCE_UART_ERROR(UART1));
                //println_E("&@&@&&@&@&@ wtf tx");
    }
    if (INTGetFlag(INT_SOURCE_UART_TX(UART1))) {
            INTClearFlag(INT_SOURCE_UART_TX(UART1));
            //println_E("&@&@&&@&@&@ wtf tx");
                    //initCoProcUART();
    }
    if (INTGetFlag(INT_SOURCE_UART(UART1))) {
            INTClearFlag(INT_SOURCE_UART(UART1));
            //println_E("&@&@&&@&@&@ generic uart");
                    //initCoProcUART();
    }

}

uint32_t CalcBaud(uint32_t Baud) {
    uint32_t closest;
    closest = ((GetPeripheralClock() + 8ul * Baud) / 16 / Baud - 1);
    return closest;
}

void Pic32UARTSetBaud(int baud) {
//    if(UARTinitialized && cachedbaud==baud)
//        return;
    cachedbaud=baud;

        //Disable first to clear
    INTEnable(INT_SOURCE_UART(UART1), INT_DISABLED);
    INTEnable(INT_SOURCE_UART_TX(UART1), INT_DISABLED);
    INTEnable(INT_SOURCE_UART_ERROR(UART1), INT_DISABLED);
    INTEnable(INT_SOURCE_UART_RX(UART1), INT_DISABLED);

    INTClearFlag(INT_SOURCE_UART_ERROR(UART1));
    INTClearFlag(INT_SOURCE_UART_TX(UART1));
    INTClearFlag(INT_SOURCE_UART_RX(UART1));
    INTClearFlag(INT_SOURCE_UART(UART1));
    CloseUART1();


   //Start configuration
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY  );
    UARTSetFifoMode(UART1, UART_INTERRUPT_ON_TX_NOT_FULL |UART_INTERRUPT_ON_RX_NOT_EMPTY);

    //OpenUART1(UART_EN|UART_EVEN_PAR_8BIT|UART_1STOPBIT|UART_DIS_BCLK_CTS_RTS,UART_TX_ENABLE|UART_RX_ENABLE,CalcBaud(INTERNAL_BAUD ));
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
     UARTSetDataRate(UART1, GetPeripheralClock(),cachedbaud);
    //U1BRG = 173;
    //UARTSetDataRate(UART1, GetPeripheralClock(), cachedbaud);
//    float actual = ((float)GetPeripheralClock()/(4.0*(((float)calculated)+1.0)));
//
//    float percent = ( actual / ((float) INTERNAL_BAUD))*100.0f;
//    if (actual != INTERNAL_BAUD) {
//        println_E("###Uart baud not what was set!! Actual = ");
//        p_fl_E(actual);
//        print_E(", desired = ");
//        p_int_E(INTERNAL_BAUD);
//        print_E(" ");
//        p_fl_E(percent);
//        print_E("%, U1BRG = ");
//        p_int_E(calculated);
//    }
    UARTEnable(UART1, UART_ENABLE_FLAGS(
            UART_PERIPHERAL |
            UART_RX | UART_TX));


    INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
    INTEnable(INT_SOURCE_UART_ERROR(UART1), INT_ENABLED);

    INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART1), INT_SUB_PRIORITY_LEVEL_0);

    mPORTDOpenDrainOpen(BIT_3);
    setPicIOTristate(OUTPUT, 'D', 3);
    ConfigUARTRXTristate();
    UARTinitialized = true;
}

void Pic32UART_HAL_INIT(int BAUDRATE) {
    Pic32UARTSetBaud(BAUDRATE);
}

void Pic32UARTGetArray(uint8_t *packet, uint16_t size) {

    INTEnable(INT_SOURCE_UART_RX(UART1), INT_DISABLED);
    FifoGetByteStream(my_store, packet, size);
    INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
}

uint16_t Pic32Get_UART_Byte_Count(void) {
    return FifoGetByteCount(my_store);
}

void Pic32UARTPutArray(uint8_t *packet, uint16_t size) {
    //return;
    //StartCritical();
    int err = UART1GetErrors();
    if (err) {
        UART1ClearAllErrors();
        UARTinitialized=false;
        Pic32UARTSetBaud(cachedbaud);
    }
    uint16_t i;
    for (i = 0; i < size && i < BOWLER_PacketSize; i++) {
        WriteUART_COM(packet[i]);
    }

    //EndCritical();
    //SetGreen(0);
}


