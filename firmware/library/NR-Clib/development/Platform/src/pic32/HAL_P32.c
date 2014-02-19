/*
 * HAL.c
 *
 *  Created on: Jan 4, 2010
 *      Author: hephaestus
 */
#include "arch/pic32/BowlerConfig.h"
#include "Bowler/Bowler.h"


typedef enum {
    EXCEP_IRQ = 0,          // interrupt
    EXCEP_AdEL = 4,         // address error exception (load or ifetch)
    EXCEP_AdES=5,             // address error exception (store)
    EXCEP_IBE=6,              // bus error (ifetch)
    EXCEP_DBE=7,              // bus error (load/store)
    EXCEP_Sys=8,              // syscall
    EXCEP_Bp=9,               // breakpoint
    EXCEP_RI=10,               // reserved instruction
    EXCEP_CpU=11,              // coprocessor unusable
    EXCEP_Overflow=12,         // arithmetic overflow
    EXCEP_Trap=13,             // trap (possible divide by zero)
    EXCEP_IS1 = 16,         // implementation specfic 1
    EXCEP_CEU=17,              // CorExtend Unuseable
    EXCEP_C2E=18               // coprocessor 2
} excep_code;

typedef union {
    UINT32 value;
    struct{
        unsigned :2;//reserved
        unsigned EXCCODE :5;//Exception code
        unsigned :1;//reserved
        unsigned IP:2;//Controls request for software interrupt
        unsigned RIPL :6;//Request Interupt Priority Level
        unsigned :6;//reserved
        unsigned R1 :1;//undocumented
        unsigned IV :1;//Interrupt Vector Bit
        unsigned :2;//reserved
        unsigned R2 :1;//undocumented
        unsigned DC :1;//Disable Count
        unsigned CE:2;// Coprocessor Exception Bits
        unsigned TI :1;// Timer Interrupt
        unsigned BD :1;// Branch Delay
    };

}cause_union;
void _general_exception_handler(unsigned cause, unsigned status){
        unsigned int x = 0;
        //asm volatile ("move %0, $ra" : "=r" (x));
        asm volatile ("sw $ra, %0" : "=m" (x));
//        /register unsigned int cp0count asm ("c0r1");

        cause_union u;
        u.value=cause;

        //setLed(1,0,0);
        setPrintLevelInfoPrint();

        print_E("\r\n\r\n\r\nException handeler!! ");
//        prHEX32(cause,INFO_PRINT);print_I(" status=");
//        prHEX32(status,INFO_PRINT);
//        print_I(" exec code=");
//        p_int_I(u.EXCCODE);
//        print_I(" return address=");
//        prHEX32(x,INFO_PRINT);
//        println_I("===    CAUSE val Parsed    ===\n\r");
//        print_I("31:\t");prHEX32(u.BD,INFO_PRINT);print_I("\t Exception During Branch Delay\r\n");
//        print_I("30:\t");prHEX32(u.TI,INFO_PRINT);print_I("\t Exception During Pending Timer Interupt\r\n");
//        print_I("29:28:\t");prHEX32(u.CE,INFO_PRINT);print_I("\t Coproc Ref number fpr a coprpc unisable exception\r\n");
//        print_I("27:\t");prHEX32(u.DC,INFO_PRINT);print_I("\t Disable Count Register\r\n");
//        print_I("23:\t");prHEX32(u.IV,INFO_PRINT);print_I("\t Special Interrupt Vector used\r\n");
//        print_I("15:10:\t");prHEX32(u.RIPL,INFO_PRINT);print_I("\t Pending Interrupt indicator or Requested Interrupt Priority Level\r\n");
//        print_I("6:2:\t");prHEX32(u.EXCCODE,INFO_PRINT);print_I("\t Exception Code\r\n");
//        switch(u.EXCCODE){
//            case 0: print_I("\t\tInterrupt\r\n"); break;
//            case 4: print_I("\t\tAddress Error Exception (Load or Instruction Fetch)\r\n"); break;
//            case 5: print_I("\t\tAddress Error Exception (Store)\r\n"); break;
//            case 6: print_I("\t\tBus Error Exception (Instruction Fetch)\r\n"); break;
//            case 7: print_I("\t\tBus Error Exception (Data Reference: load or store)\r\n"); break;
//            case 8: print_I("\t\tSyscall Exception\r\n"); break;
//            case 9: print_I("\t\tBreakpoint Exception\r\n"); break;
//            case 10: print_I("\t\tReversed Instruction Exception\r\n"); break;
//            case 11: print_I("\t\tCoProcessor Unusable exception\r\n"); break;
//            case 12: print_I("\t\tArithmatic Overflow Exception\r\n"); break;
//            case 13: print_I("\t\tTrap Exception\r\n"); break;
//            case 16: print_I("\t\tImplementation Specific Exception (COP2)\r\n"); break;
//            case 17: print_I("\t\tCorExtend Unusable\r\n"); break;
//            case 18: print_I("\t\tCoprocessor 2 Exceptions\r\n"); break;
//            default: print_I("\t\treserved\r\n"); break;
//
//        }
//
//        println_I("=== CoProc 0 Register Dump ===");
//        println_I(" BadVAddr=\t");
//        prHEX32(_CP0_GET_BADVADDR(),INFO_PRINT);
//        println_I(" Compare=\t");
//        prHEX32(_CP0_GET_COMPARE(),INFO_PRINT);
//        println_I(" Count=\t");
//        prHEX32(_CP0_GET_COUNT(),INFO_PRINT);
//        println_I(" Status=\t");
//        prHEX32(_CP0_GET_STATUS(),INFO_PRINT);
//        println_I(" EPC=\t");
//        prHEX32(_CP0_GET_EPC(),INFO_PRINT);
//        println_I(" PRID=\t");
//        prHEX32(_CP0_GET_PRID(),INFO_PRINT);
//        println_I(" EBASE=\t");
//        prHEX32(_CP0_GET_EBASE(),INFO_PRINT);
//        println_I(" CONFIG=\t");
//        prHEX32(_CP0_GET_CONFIG(),INFO_PRINT);
//        println_I(" ERROREPC=\t");
//        prHEX32(_CP0_GET_DEPC(),INFO_PRINT);
//        println_I(" ERROREPC=\t");
//        prHEX32(_CP0_GET_DEPC(),INFO_PRINT);
//        print_I(" \r\n\r\n\r\n");

        DelayMs(1000);
        Reset();
}

static BYTE_FIFO_STORAGE storeUSB;
static BYTE privateRXUSB[BOWLER_PacketSize];
static BYTE_FIFO_STORAGE storeUART;
static BYTE privateRXUART[BOWLER_PacketSize];

static BOOL init=FALSE;

static BOOL usbComs = FALSE;
static BOOL uartComs = FALSE;

static BOOL disableSerial=FALSE;

void disableSerialComs(BOOL state){
    disableSerial=state;
    uartComs = !state;
}
void Pic32_Bowler_HAL_Init(void){
	TickInit();
	init=TRUE;
	//println_W("Init UART hal");
	Pic32UART_HAL_INIT(PRINT_BAUD);
	//println_W("Init USB fifo");
	InitByteFifo(&storeUSB,privateRXUSB,sizeof(privateRXUSB));
	//println_W("Init UART fifo");
	InitByteFifo(&storeUART,privateRXUART,sizeof(privateRXUART));
	//println_W("Setting Serial FIFO buffer");
	SetPICUARTFifo(&storeUART);
	//println_W("Setting USB FIFO buffer");
	SetPICUSBFifo(&storeUSB);
	//println_W("Pic32 is initialized...");
}

//HAL init functions

void Get_HAL_Packet(BYTE * packet,WORD size){

    if(usbComs){
        if(FifoGetByteStream(&storeUSB,packet,size) != 0)
            return;
    }

    if(uartComs){
        if(FifoGetByteStream(&storeUART,packet,size) != 0)
            return;
    }
}

BOOL Send_HAL_Packet(BYTE * packet,WORD size){

    if(usbComs)
        SendPacketUSB(packet,size);
    if(uartComs)
        Pic32UARTPutArray(packet,size);
    return TRUE;
}
WORD Get_HAL_Byte_Count(){
#if defined(USB_POLLING)
        USBDeviceTasks();
#endif
	if( init==FALSE){
		//println("***Initializing the PIC hal***");
		Pic32_Bowler_HAL_Init();
	}
	GetNumUSBBytes();//This runs other update tasks for the USB stack

	if(GetNumUSBBytes()>0){
            usbComs=TRUE;
            return FifoGetByteCount(&storeUSB);
	}
	else if(Pic32Get_UART_Byte_Count()>0){
            if(!disableSerial)
                uartComs=TRUE;
            return FifoGetByteCount(&storeUART);
	}
	return 0;
}

BOOL GetBowlerPacket_arch(BowlerPacket * Packet){
	Get_HAL_Byte_Count();//This runs other update tasks for the HAL
        if(usbComs)
            if(GetBowlerPacketDebug(Packet,&storeUSB))
                return TRUE;
        if (uartComs)
            if(GetBowlerPacketDebug(Packet,&storeUART))
                return TRUE;
	return FALSE;
}


/**
 * send the array out the connection
 */
UINT16 putStream(BYTE *packet,UINT16 size){
	Send_HAL_Packet(packet,size);
	return TRUE;
}
/**
 * get the time in ms
 */
float getMs(void){
	return TickGetMS();
}
/**
 * send this char to the //print terminal
 */
void putCharDebug(char c){
	WriteUART_DEBUG(c);
}
/**
 * Start the scheduler
 */
void startScheduler(void){
	TickInit();
}


void EnableDebugTerminal(void){
	Pic32UART_HAL_INIT(PRINT_BAUD);
}

