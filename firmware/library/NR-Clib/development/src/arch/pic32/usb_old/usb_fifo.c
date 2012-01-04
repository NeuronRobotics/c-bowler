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

#include "USB/usb_fifo.h"
#include "Bowler/Bowler.h"

#define USBNotOk	(USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)

#define TxPrivateSize 64
static BYTE RxTmpBuffer[BOWLER_PacketSize];
static BYTE privateRX[BOWLER_PacketSize];
static BYTE TxBuffer[TxPrivateSize];
static UINT16 gotData = 0;
static BOOL bufferSet=FALSE;

static WORD     txSize;


static BYTE_FIFO_STORAGE store;
static BYTE_FIFO_STORAGE * my_store;

static BOOL usbActive = FALSE;

BOOL GotUSBData(void){
	return gotData>0;
}

BYTE_FIFO_STORAGE  * GetPICUSBFifo(void){
	return my_store;
}

void SetPICUSBFifo(BYTE_FIFO_STORAGE  * s){
	if(bufferSet==TRUE)
		return;
	bufferSet=TRUE;
	my_store=s;
}

void usb_CDC_Serial_Init(char * DevStr,char * SerialStr,UINT16 vid,UINT16 pid){

	unsigned char i;
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
	InitByteFifo(&store,privateRX,sizeof(privateRX));
	if(bufferSet==FALSE)
		my_store=&store;
	#if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif
    INTEnableSystemMultiVectoredInt();
    usb_Buffer_Update();
}

WORD USBGetArray(BYTE* stream, WORD num){
	if(USBNotOk){
		return  0;
	}
	usb_Buffer_Update();
	gotData-=num;
	BYTE n = FifoGetByteStream(my_store,stream,num);
	return n;
}
//static BYTE tmp [64];
extern BYTE cdc_trf_state;
void flush(){
	RunEveryData timeout={getMs(),200};
	CDCTxService();
	while((cdc_trf_state != CDC_TX_READY)){
		if(RunEvery(&timeout)>0){
			println("#*#*USB timeout before transmit");
			usbActive=FALSE;
			return;
		}
		if(USBNotOk){
			println("#*#*USB Not ok");
			return;
		}
		CDCTxService();
	}
	if(txSize>0){
		putUSBUSART((char *)TxBuffer, txSize);
		txSize=0;
	}
	CDCTxService();
	while((cdc_trf_state != CDC_TX_READY)){
		if(RunEvery(&timeout)>0){
			println("#*#*USB timeout before transmit");
			usbActive=FALSE;
			return;
		}
		if(USBNotOk){
			println("#*#*USB Not ok");
			return;
		}
		CDCTxService();
	}

}
WORD USBPutArray(BYTE* stream, WORD num){
	if(usbActive==FALSE){
		return 0;
	}
	//UINT16 i;

	usb_Buffer_Update();
	if(USBNotOk){
		return  0;
	}else{
		int packetLen = num;
		int packetIndex = 0;
		int i;
		if(num>TxPrivateSize) {
			while(packetLen>TxPrivateSize) {
				for(i=0;i<TxPrivateSize;i++) {
					TxBuffer[i]=stream[packetIndex++];
					packetLen--;
				}
				num=i;
				println("Sending chunk ");printByteArray(TxBuffer,num);
				flush();

			}
			for(i=0;i<packetLen;i++) {
				TxBuffer[i]=stream[packetIndex++];
			}
			num=i;
			//println("Sending chunk ");printByteArray(TxBuffer,num);
			flush();
		}else {
			for (i=0;i<num;i++){
				TxBuffer[i]=stream[i];
			}
			txSize=num;
			//println("Sending all ");printByteArray(TxBuffer,num);
			flush();
		}
	}
	return TRUE;
}


WORD GetNumUSBBytes(void){
	usb_Buffer_Update();
	return FifoGetByteCount(my_store);
}


void usb_Buffer_Update(void){
	if(USBNotOk){
		return ;
	}
	WORD i;
	WORD gSize=getsUSBUSART((char *)RxTmpBuffer, USB_BUFFER_SIZE);
	BYTE err;
	if (gSize>0){
		for(i=0;i<gSize;i++){
			do{
				FifoAddByte(my_store,RxTmpBuffer[i], & err);
			}while(err != FIFO_OK );
			gotData++;
			usbActive = TRUE;
		}
		//p_ul(my_store->byteCount);
	}

}

