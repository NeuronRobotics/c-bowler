/*
 * HAL.c
 *
 *  Created on: Jan 4, 2010
 *      Author: hephaestus
 */
#include "Bowler/Bowler.h"

static BYTE_FIFO_STORAGE storeUSB;
static BYTE privateRXUSB[BOWLER_PacketSize];
static BYTE_FIFO_STORAGE storeUART;
static BYTE privateRXUART[BOWLER_PacketSize];
typedef enum _hal_state
{
	NO_WASP_PACKET=0,
    UART_WASP_PACKET,
    USB_WASP_PACKET
} HAL_WASP_SWITCH;

static HAL_WASP_SWITCH HalSwitch;
static BOOL init=FALSE;


void Pic32_Bowler_HAL_Init(void){
	init=TRUE;
	//println("Init UART hal");
	Pic32UART_HAL_INIT();
	//println("Init USB fifo");
	InitByteFifo(&storeUSB,privateRXUSB,sizeof(privateRXUSB));
	//println("Init UART fifo");
	InitByteFifo(&storeUART,privateRXUART,sizeof(privateRXUART));
	HalSwitch= NO_WASP_PACKET;
	//println("Setting Serial FIFO buffer");
	SetPICUARTFifo(&storeUART);
	//println("Setting USB FIFO buffer");
	SetPICUSBFifo(&storeUSB);
	//println("Pic32 is initialized...");
}

//HAL init functions

void Get_HAL_Packet(BYTE * packet,WORD size){
	switch (HalSwitch){
	case UART_WASP_PACKET:
		FifoGetByteStream(&storeUART,packet,size);
		break;
	case USB_WASP_PACKET:
		FifoGetByteStream(&storeUSB,packet,size);
		break;
	case NO_WASP_PACKET:
		break;
	}
}

BOOL Send_HAL_Packet(BYTE * packet,WORD size){
	//SetColor(1,0,0);
	switch (HalSwitch){
	case UART_WASP_PACKET:
		////println("Sending to UART");
		Pic32UARTPutArray(packet,size);
		break;
	case USB_WASP_PACKET:
		////println("Sending to USB");
		SendPacketUSB(packet,size);
		break;
	default:
		//println("Not sending upstream");
		break;
	}
	return TRUE;
}
WORD Get_HAL_Byte_Count(){
	if( init==FALSE){
		//println("***Initializing the PIC hal***");
		Pic32_Bowler_HAL_Init();
	}
	GetNumUSBBytes();//This runs other update tasks for the USB stack

	if(GetNumUSBBytes()>0){
		HalSwitch =USB_WASP_PACKET;
		return FifoGetByteCount(&storeUSB);
	}
	else if(Pic32Get_UART_Byte_Count()>0){
		HalSwitch =UART_WASP_PACKET;
		//return storeUART.byteCount;
		return FifoGetByteCount(&storeUART);
	}
	return 0;
}

BOOL GetBowlerPacket_arch(BowlerPacket * Packet){
	Get_HAL_Byte_Count();//This runs other update tasks for the HAL
	switch (HalSwitch){
	case UART_WASP_PACKET:
		return GetBowlerPacket(Packet,&storeUART);
	case USB_WASP_PACKET:
		return GetBowlerPacket(Packet,&storeUSB);
	case NO_WASP_PACKET:
		break;
	}
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
	Pic32UART_HAL_INIT();
}

