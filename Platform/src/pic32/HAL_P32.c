/*
 * HAL.c
 *
 *  Created on: Jan 4, 2010
 *      Author: hephaestus
 */
#include "arch/pic32/BowlerConfig.h"
#include "Bowler/Bowler.h"

enum {
    EXCEP_IRQ = 0, // interrupt
    EXCEP_AdEL = 4, // address error exception (load or ifetch)
    EXCEP_AdES, // address error exception (store)
    EXCEP_IBE, // bus error (ifetch)
    EXCEP_DBE, // bus error (load/store)
    EXCEP_Sys, // syscall
    EXCEP_Bp, // breakpoint
    EXCEP_RI, // reserved instruction
    EXCEP_CpU, // coprocessor unusable
    EXCEP_Overflow, // arithmetic overflow
    EXCEP_Trap, // trap (possible divide by zero)
    EXCEP_IS1 = 16, // implementation specfic 1
    EXCEP_CEU, // CorExtend Unuseable
    EXCEP_C2E // coprocessor 2
} _excep_code;

void __attribute__((nomips16)) _general_exception_handler() {
    //  unsigned int _epc_code;
    unsigned int _excep_addr;
    asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
    asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));
    _excep_code = (_excep_code & 0x0000007C) >> 2;

    setPrintLevelInfoPrint();

    print_E("\r\nException ");
    switch (_excep_code) {
        case EXCEP_IRQ: print_E("interrupt");
            break;
        case EXCEP_AdEL: print_E("address error exception (load or ifetch)");
            break;
        case EXCEP_AdES: print_E("address error exception (store)");
            break;
        case EXCEP_IBE: print_E("bus error (ifetch)");
            break;
        case EXCEP_DBE: print_E("bus error (load/store)");
            break;
        case EXCEP_Sys: print_E("syscall");
            break;
        case EXCEP_Bp: print_E("breakpoint");
            break;
        case EXCEP_RI: print_E("reserved instruction");
            break;
        case EXCEP_CpU: print_E("coprocessor unusable");
            break;
        case EXCEP_Overflow: print_E("arithmetic overflow");
            break;
        case EXCEP_Trap: print_E("trap (possible divide by zero)");
            break;
        case EXCEP_IS1: print_E("implementation specfic 1");
            break;
        case EXCEP_CEU: print_E("CorExtend Unuseable");
            break;
        case EXCEP_C2E: print_E("coprocessor 2");
            break;
    }
    print_E(" at 0x");
    prHEX32(_excep_addr, ERROR_PRINT);
    print_E("\r\n");

    DelayMs(1000);
    Reset();
}

BYTE_FIFO_STORAGE storeUSB;
uint8_t privateRXUSB[BOWLER_PacketSize ];
  BYTE_FIFO_STORAGE storeUART;
  uint8_t privateRXUART[BOWLER_PacketSize];

boolean isPic32Initialized = false;

 boolean usbComs = false;
  boolean uartComs = false;

  boolean disableSerial = true;

void disableSerialComs(boolean state) {
    disableSerial = state;
    uartComs = !state;
}

void Pic32_Bowler_HAL_Init(void) {

    isPic32Initialized = true;
//    println_W("Init ADC");
//    int i = 0;
//    for (i = 0; i < 16; i++) InitADCHardware(i);
//    measureAdcOffset();
    //println_W("Init USB fifo");
    InitByteFifo(&storeUSB, privateRXUSB, sizeof (privateRXUSB));
//    println_W("Init UART fifo");
    InitByteFifo(&storeUART, privateRXUART, sizeof (privateRXUART));
//  println_W("Setting Serial FIFO buffer");
    SetPICUARTFifo(&storeUART);
//    println_W("Setting USB FIFO buffer");
    SetPICUSBFifo(&storeUSB);

    //println_W("Init UART hal");
    //Pic32UART_HAL_INIT(PRINT_BAUD);
    TickInit();

    //println_W("Pic32 is initialized...");
}

void setPicIOTristateInput(char port,int pin){
    setPicIOTristate( INPUT, port, pin);
}
void setPicIOTristateOutput(char port,int pin){
    setPicIOTristate( OUTPUT, port, pin);
}
void setPicIOTristate(boolean state,char port,int pin){
    switch (port) {
        case 'B':
            if(state){TRISBSET=(1<<pin);}else{ TRISBCLR=(1<<pin);}
            break;
        case 'C':
            if(state){TRISCSET=(1<<pin);}else{ TRISCCLR=(1<<pin);}
            break;
        case 'D':
            if(state){TRISDSET=(1<<pin);}else{ TRISDCLR=(1<<pin);}
            break;
        case 'E':
            if(state){TRISESET=(1<<pin);}else{ TRISECLR=(1<<pin);}
            break;
        case 'F':
            if(state){TRISFSET=(1<<pin);}else{ TRISFCLR=(1<<pin);}
            break;
        case 'G':
            if(state){TRISGSET=(1<<pin);}else{ TRISGCLR=(1<<pin);}
            break;
        default:
            println_E("INVALID PIN ID");
            while(1);
    }
}

boolean getPicIOPin(char port,int pin){
    switch (port) {
        case 'B':
            return  PORTB & (1<<pin)?1:0;
        case 'C':
            return  PORTC & (1<<pin)?1:0;
        case 'D':
            return  PORTD & (1<<pin)?1:0;
        case 'E':
            return  PORTE & (1<<pin)?1:0;
        case 'F':
            return  PORTF & (1<<pin)?1:0;
        case 'G':
            return  PORTG & (1<<pin)?1:0;
        default:
            println_E("INVALID PIN ID");
            while(1);
    }
}

void setPicIOPin(boolean state, char port, int pin) {
    switch (port) {
        case 'B':
            ioPortB(state, pin);
            break;
        case 'C':
            ioPortC(state, pin);
            break;
        case 'D':
            ioPortD(state, pin);
            break;
        case 'E':
            ioPortE(state, pin);
            break;
        case 'F':
            ioPortF(state, pin);
            break;
        case 'G':
            ioPortG(state, pin);
            break;
        default:
            println_E("INVALID PIN ID");
            while(1);
    }
}

//HAL init functions

void Get_HAL_Packet(uint8_t * packet, uint16_t size) {

    if (usbComs) {
        if (FifoGetByteStream(&storeUSB, packet, size) != 0)
            return;
    }

    if (uartComs) {
        if (FifoGetByteStream(&storeUART, packet, size) != 0)
            return;
    }

}

boolean Send_HAL_Packet(uint8_t * packet, uint16_t size) {

    if (usbComs)
        SendPacketUSB(packet, size);
    if (uartComs)
        Pic32UARTPutArray(packet, size);
    return true;
}

uint16_t Get_HAL_Byte_Count() {
#if defined(USB_POLLING)
    USBDeviceTasks();
#endif
    if (isPic32Initialized == false) {
        //println_W("***Initializing the PIC hal***");
        Pic32_Bowler_HAL_Init();
    }
    //println_I("Getting the USB bytes");

    if (GetNumUSBBytes() > 0) {
        usbComs = true;
        //println_I("Found USB bytes");
        return FifoGetByteCount(&storeUSB);
    } else {
        //println_I("Getting the UART bytes");
        if (Pic32Get_UART_Byte_Count() > 0) {
            //println_I("Found the UART bytes");
            if (!disableSerial)
                uartComs = true;
            return FifoGetByteCount(&storeUART);
        }
    }
    return 0;
}

boolean GetBowlerPacket_arch(BowlerPacket * Packet) {
    //println_I("Checking packet from HAL");
    Get_HAL_Byte_Count(); //This runs other update tasks for the HAL
    //println_I("Getting packet from HAL");
    if (usbComs)
        if (GetBowlerPacketDebug(Packet, &storeUSB))
            return true;
    if (uartComs)
        if (GetBowlerPacketDebug(Packet, &storeUART))
            return true;
    return false;
}

/**
 * send the array out the connection
 */
uint16_t putStream(uint8_t *packet, uint16_t size) {
    Print_Level l = getPrintLevel();
    setPrintLevelInfoPrint();
    println_I("UP>> ");printPacket((BowlerPacket *)packet,INFO_PRINT);
    setPrintLevel(l);
    Send_HAL_Packet(packet, size);
    return true;
}

/**
 * get the time in ms
 */
float getMs(void) {
    return TickGetMS();
}

/**
 * send this char to the //print terminal
 */
void putCharDebug(char c) {
    WriteUART_DEBUG(c);
}

/**
 * Start the scheduler
 */
void startScheduler(void) {
    TickInit();
}

void EnableDebugTerminal(void) {
    Pic32UART_HAL_INIT(PRINT_BAUD);
}

