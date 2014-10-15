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

#define TxPrivateSize CDC_DATA_OUT_EP_SIZE
uint8_t USBRxTmpBuffer[BOWLER_PacketSize];
uint8_t USBTxBuffer[TxPrivateSize];
uint16_t gotDataUSB = 0;
boolean bufferSetUSB = false;

uint16_t txSizeUSB;


//static BYTE_FIFO_STORAGE store;
BYTE_FIFO_STORAGE * usb_fifo_my_store = NULL;
BYTE_FIFO_STORAGE * last_my_store = NULL;

boolean usbActive = true;

//static BYTE tmp [64];
//extern BYTE cdc_trf_state;
//extern USB_HANDLE CDCDataInHandle;



BOOL GotUSBData(void) {
    return gotDataUSB > 0;
}

void printBufferState(BYTE_FIFO_STORAGE * s) {
    println_E("\tFIFO state ");
    p_int_E((int) s);
    println_E("\tBuffer state ");
    p_int_E((int) s->buffer);
    println_E("\tBuffer size ");
    p_int_E((int) s->bufferSize);
    println_E("\tBuffer count ");
    p_int_E((int) s->byteCount);
    println_E("\tRead Pointer ");
    p_int_E((int) s->readPointer);
    println_E("\tWrite Pointer ");
    p_int_E((int) s->writePointer);
}

BYTE_FIFO_STORAGE * GetPICUSBFifo(void) {
    if (usb_fifo_my_store == NULL || usb_fifo_my_store != last_my_store) {
        setPrintLevelInfoPrint();
        println_E("Usb storage changed!! was");
        printBufferState(last_my_store);
        println_E("Is: ");
        printBufferState(usb_fifo_my_store);
        while (1);
    }
    last_my_store = usb_fifo_my_store;
    return usb_fifo_my_store;
}

void SetPICUSBFifo(BYTE_FIFO_STORAGE * s) {
    Print_Level l = getPrintLevel();
    setPrintLevelInfoPrint();
    println_E("Starting To set FIFO ");
    if (bufferSetUSB == true)
        return;
    bufferSetUSB = true;
    //printBufferState(s);
    usb_fifo_my_store = s;
    last_my_store = s;
    //printBufferState(GetPICUSBFifo());
    setPrintLevel(l);

}

void usb_CDC_Serial_Init(char * DevStr, char * SerialStr, uint16_t vid, uint16_t pid) {

    if (usb_fifo_my_store == NULL) {
        println_E("USB fifo is null, can not initialize");
        while (1);
    }

    //unsigned char i;
    DelayMs(100);
    SetUSB_VID_PID(vid, pid);
    WriteUSBSerialNumber(SerialStr);
    WriteUSBDeviceString(DevStr);
#if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
#endif
#if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN; // See HardwareProfile.h
#endif
    USBDeviceInit();
    //InitByteFifo(&store,privateRX,sizeof(privateRX));
    //	if(bufferSet==FALSE)
    //		usb_fifo_my_store=&store;
#if defined(USB_INTERRUPT)
    USBDeviceAttach();
#endif
    INTEnableSystemMultiVectoredInt();

    GetNumUSBBytes();
    println_I("Initialized the USB with: ");
    print_I(DevStr);
    print_I(" ");
    print_I(SerialStr);
    print_I(" ");
    prHEX16(vid, INFO_PRINT);
    print_I(":");
    prHEX16(pid, INFO_PRINT);
    int i;
    int j='a';
    for(i=0;i<TxPrivateSize;i++){

       USBTxBuffer[i] = j;// write the alphabet into the buffer
       j++;
       if(j>'z')
           j='a';
    }
    print_E("\r\nData dumped");

}

uint16_t USBGetArray(uint8_t* stream, uint16_t num) {
    if (USBNotOk) {
        return 0;
    }
    usb_Buffer_Update();
    gotDataUSB -= num;

    BYTE n = FifoGetByteStream(GetPICUSBFifo(), stream, num);
    return n;
}


void waitForTxToBeFree() {
    RunEveryData timeout = {getMs(), 200};
    while (isUSBTxBlocked()) {
    	//println_I("USB blocked");
        if (RunEvery(&timeout) > 0) {
            println_E("#*#*USB timeout before transmit");
            usbActive = false;
            break;
        }
        if (USBNotOk) {
            println_E("#*#*USB Not ok");
            usbActive = false;
            break;
        }
        CDCTxService();
    }

}

void flush() {

    float start = getMs();


    waitForTxToBeFree();

    float end = getMs() - start;
    //println_I("USB Buffer ready, took:");p_fl_I(end);
    start = getMs();


    if (txSizeUSB > 0) {
        putUSBUSART((char *) USBTxBuffer, txSizeUSB);
        //DelayMs(2);
        txSizeUSB = 0;
    } else {
        //println_I("Zero length packet to send, ignoring");
    }

    waitForTxToBeFree();

    end = getMs() - start;
    //println_I("USB Flushed OK, took:");p_fl_I(end);
}

boolean isUSBActave() {
    return usbActive;
}

void forceOpenUSB() {
    usbActive = true;
}

int USBPutArray(uint8_t* stream, int num) {
    if (isUSBActave() == false) {
        //println_I("USB inactive, bailing out");
        return 0;
    }
    //uint16_t i;

    usb_Buffer_Update();
    if (USBNotOk) {
        usbActive = false;
        return 0;
    } else {
        int packetLen = num;
        int packetIndex = 0;
        int i;
        //if(num>(TxPrivateSize)) {
        //print_E("\r\nAttempting");printStream_E(stream,num);
        if (num > TxPrivateSize) {
            //println_I("Packet too large for USB buffer");
            while (packetLen > TxPrivateSize) {
                for (i = 0; i < TxPrivateSize; i++) {
                    USBTxBuffer[i] = stream[packetIndex++];
                    packetLen--;
                }
                //println_I("Sending chunk ");printStream_I(USBTxBuffer,i);
                txSizeUSB = i;
                flush();
            }
            for (i = 0; i < packetLen; i++) {
                USBTxBuffer[i] = stream[packetIndex++];
            }
            //println_I("Sending chunk ");printStream_I(USBTxBuffer,i);
            txSizeUSB = i;
        } else {
            //println_I("Packet small enough for USB buffer");
            for (i = 0; i < num; i++) {
                USBTxBuffer[i] = stream[i];
            }
            //println_I("Sending all ");printStream_I(USBTxBuffer,num);
            txSizeUSB = i;
        }
    }
    flush();
    return TRUE;
}

uint16_t GetNumUSBBytes(void) {
    usb_Buffer_Update();
    //printBufferState(GetPICUSBFifo());
    //println_I("Update Buffer = ");
    //	BYTE_FIFO_STORAGE* fifo = GetPICUSBFifo();

    uint16_t data = FifoGetByteCount(GetPICUSBFifo());
    //p_int_I(data);
    return data;
}

void usb_Buffer_Update(void) {
    if (USBNotOk) {
        usbActive = FALSE;
        return;
    }
    uint16_t i;
    uint16_t gSize = getsUSBUSART((char *) USBRxTmpBuffer, USB_BUFFER_SIZE);
    BYTE err;
    if (gSize > 0) {
        for (i = 0; i < gSize; i++) {
            do {
                FifoAddByte(GetPICUSBFifo(), USBRxTmpBuffer[i], & err);
            } while (err != FIFO_OK);
            gotDataUSB++;
            usbActive = TRUE;
        }
    }

}

