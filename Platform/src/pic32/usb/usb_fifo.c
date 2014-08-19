/*
 * usb_fifo.c
 *
 *  Created on: Jun 3, 2010
 *      Author: hephaestus
 */

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

#include "arch/pic32/USB/usb_fifo.h"
#include "Bowler/Bowler.h"

#define USBNotOk	(USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)

#define TxPrivateSize 64
static uint8_t RxTmpBuffer[BOWLER_PacketSize];
//static uint8_t privateRX[BOWLER_PacketSize];
static uint8_t TxBuffer[TxPrivateSize+1 ];
static uint16_t gotData = 0;
static boolean bufferSet=FALSE;

static uint16_t     txSize;


//static BYTE_FIFO_STORAGE store;
static BYTE_FIFO_STORAGE * usb_fifo_my_store=NULL;
static BYTE_FIFO_STORAGE * last_my_store=NULL;

static boolean usbActive = TRUE;

boolean GotUSBData(void){
	return gotData>0;
}

void printBufferState(BYTE_FIFO_STORAGE  * s){
	println_E("\tFIFO state 0x");prHEX32((int)s,ERROR_PRINT);
        println_E("\tTo  0x");prHEX32(((int)s+sizeof(BYTE_FIFO_STORAGE)),ERROR_PRINT);
	println_E("\tBuffer state 0x");prHEX32((int)s->buffer,ERROR_PRINT);
	println_E("\tBuffer size ");p_int_E((int)s->bufferSize);
	println_E("\tBuffer count ");p_int_E((int)s->byteCount);
	println_E("\tRead Pointer ");p_int_E((int)s->readPointer);
	println_E("\tWrite Pointer ");p_int_E((int)s->writePointer);
}

BYTE_FIFO_STORAGE  * GetPICUSBFifo(void){
	if(usb_fifo_my_store->buffer == NULL ){
		setPrintLevelInfoPrint();
		println_E("Usb storage changed!! was");printBufferState(last_my_store);
		println_E("Is: ");printBufferState(usb_fifo_my_store);
                if(usb_fifo_my_store->buffer == NULL )
                    while(1);
	}
	last_my_store=usb_fifo_my_store;
	return usb_fifo_my_store;
}



void SetPICUSBFifo(BYTE_FIFO_STORAGE  * s){
	Print_Level l = getPrintLevel();
	setPrintLevelInfoPrint();
	println_E("Starting To set FIFO ");
	if(bufferSet==TRUE)
		return;
	bufferSet=TRUE;
	//printBufferState(s);
	usb_fifo_my_store=s;
	last_my_store=s;
	//printBufferState(GetPICUSBFifo());
	setPrintLevel(l);
        GetPICUSBFifo();
        println_E("USB FIFO OK");
}

void usb_CDC_Serial_Init(char * DevStr,char * SerialStr,uint16_t vid,uint16_t pid){

	//unsigned char i;
	DelayMs(100);
	SetUSB_VID_PID(vid,pid);
	WriteUSBSerialNumber(SerialStr);
	WriteUSBDeviceString(DevStr);
	#if defined(USE_USB_BUS_SENSE_IO)
		tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
	#endif
	#if defined(USE_SELF_POWER_SENSE_IO)
		tris_self_power = INPUT_PIN;	// See HardwareProfile.h
	#endif
	USBDeviceInit();

	#if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif
    INTEnableSystemMultiVectoredInt();

	GetNumUSBBytes();
	println_I("Initialized the USB");
}

uint16_t USBGetArray(BYTE* stream, uint16_t num){
	if(USBNotOk){
		return  0;
	}
	usb_Buffer_Update();
	gotData-=num;

	uint8_t n = FifoGetByteStream(GetPICUSBFifo(),stream,num);
	return n;
}
//static uint8_t tmp [64];
extern uint8_t cdc_trf_state;
extern USB_HANDLE CDCDataInHandle;

#define isUSBTxBlocked() ((cdc_trf_state != CDC_TX_READY)  || (USBHandleBusy(CDCDataInHandle)!=0))

void waitForTxToBeFree(){
	RunEveryData timeout={getMs(),200};
	while(isUSBTxBlocked()){
		if(RunEvery(&timeout)>0){
			println_E("#*#*USB timeout before transmit");
			usbActive=FALSE;
                        break;
		}
		if(USBNotOk){
			println_E("#*#*USB Not ok");
                        usbActive=FALSE;
                        break;
		}
		CDCTxService();
	}

}

void flush(){

	float start = getMs();

	waitForTxToBeFree();

	float end = getMs()-start;
	//println_I("USB Buffer ready, took:");p_fl_I(end);
	start = getMs();


	if(txSize>0){
		putUSBUSART((char *)TxBuffer, txSize);
		//DelayMs(2);
		txSize=0;
	}else{
		//println_I("Zero length packet to send, ignoring");
	}

	waitForTxToBeFree();

	end = getMs()-start;
	//println_I("USB Flushed OK, took:");p_fl_I(end);
}

uint8_t isUSBActave(){
    return usbActive;
}

void forceOpenUSB(){
    usbActive=TRUE;
}

int USBPutArray(BYTE* stream, int num){
	if(isUSBActave()==FALSE){
		//println_I("USB inactive, bailing out");
		return 0;
	}
	//uint16_t i;

	usb_Buffer_Update();
	if(USBNotOk){
		usbActive=FALSE;
		return  0;
	}else{
		int packetLen = num;
		int packetIndex = 0;
		int i;
		//if(num>(TxPrivateSize)) {
		if(num>255) {
			//println_I("Packet too large for USB buffer");
			while(packetLen>TxPrivateSize) {
				for(i=0;i<TxPrivateSize;i++) {
					TxBuffer[i]=stream[packetIndex++];
					packetLen--;
				}
				//println_I("Sending chunk ");printStream_I(TxBuffer,i);
				txSize=i;
				flush();
			}
			for(i=0;i<packetLen;i++) {
				TxBuffer[i]=stream[packetIndex++];
			}
			//println_I("Sending chunk ");printStream_I(TxBuffer,i);
			txSize=i;
			flush();
		}else {
			//println_I("Packet small enough for USB buffer");
			for (i=0;i<num;i++){
				TxBuffer[i]=stream[i];
			}
			//println_I("Sending all ");printStream_I(TxBuffer,num);
			txSize=i;
			flush();
		}
	}
	return TRUE;
}


uint16_t GetNumUSBBytes(void){
	usb_Buffer_Update();
	//printBufferState(GetPICUSBFifo());
	//println_I("Update Buffer = ");
//	BYTE_FIFO_STORAGE* fifo = GetPICUSBFifo();

	uint16_t data = FifoGetByteCount(GetPICUSBFifo());
	//p_int_I(data);
	return data;
}


void usb_Buffer_Update(void){
	if(USBNotOk){
		usbActive=FALSE;
		return ;
	}
	uint16_t i;
	uint16_t gSize=getsUSBUSART((char *)RxTmpBuffer, USB_BUFFER_SIZE);
	uint8_t err;
	if (gSize>0){
		for(i=0;i<gSize;i++){
			do{
				FifoAddByte(GetPICUSBFifo(),RxTmpBuffer[i], & err);
			}while(err != FIFO_OK );
			gotData++;
			usbActive = TRUE;
		}
	}

}

