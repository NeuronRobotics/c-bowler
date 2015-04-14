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

#define TxPrivateSize BOWLER_PacketSize
BYTE RxTmpBuffer[BOWLER_PacketSize];
//BYTE privateRX[BOWLER_PacketSize];
BYTE TxBuffer[TxPrivateSize + 1 ];
UINT16 gotData = 0;
BOOL bufferSet = FALSE;

WORD txSize;


//BYTE_FIFO_STORAGE store;
BYTE_FIFO_STORAGE * usb_fifo_my_store = NULL;
BYTE_FIFO_STORAGE * last_my_store = NULL;

BOOL usbActive = TRUE;

//BYTE tmp [64];
extern BYTE cdc_trf_state;
extern USB_HANDLE CDCDataInHandle;//i would have called it out handle, but the stack uses In handle for tx

#define isUSBTxBlocked() ((cdc_trf_state != CDC_TX_READY)  || (USBHandleBusy(CDCDataInHandle)!=0))

BOOL GotUSBData(void) {
    return gotData > 0;
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
        //setPrintLevelInfoPrint();
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
    //setPrintLevelInfoPrint();
    println_E("Starting To set FIFO ");
    if (bufferSet == TRUE)
        return;
    bufferSet = TRUE;
    //printBufferState(s);
    usb_fifo_my_store = s;
    last_my_store = s;
    //printBufferState(GetPICUSBFifo());
    setPrintLevel(l);

}

void resetUsbSystem() {
    U1CON = 0x0000;
    DelayMs(100);

    USBDeviceInit();
    //InitByteFifo(&store,privateRX,sizeof(privateRX));
    //	if(bufferSet==FALSE)
    //		usb_fifo_my_store=&store;
#if defined(USB_INTERRUPT)
    USBDeviceAttach();
#endif
    INTEnableSystemMultiVectoredInt();

    GetNumUSBBytes();
    println_W("Initialized the USB");
}

void usb_CDC_Serial_Init(char * DevStr, char * SerialStr, UINT16 vid, UINT16 pid) {

    //unsigned char i;
    DelayMs(100);
    SetUSB_VID_PID(vid, pid);
    WriteUSBSerialNumber(SerialStr);
    WriteUSBDeviceString(DevStr);

    resetUsbSystem();

}

WORD USBGetArray(BYTE* stream, WORD num) {
    if (USBNotOk) {
        return 0;
    }
    usb_Buffer_Update();
    gotData -= num;

    BYTE n = FifoGetByteStream(GetPICUSBFifo(), stream, num);
    return n;
}

void waitForTxToBeFree() {
    //USBEnableInterrupts();
    USBUnmaskInterrupts();
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();
    
    RunEveryData timeout = {getMs(), USB_TIMEOUT};
    while (isUSBTxBlocked()) {
        Delay1us(5);
        if (RunEvery(&timeout) > 0) {
//            println_E("#*#*USB timeout before transmit");
//            if ((cdc_trf_state != CDC_TX_READY)) {
//                println_E("cdc_trf_state = ");
//                p_int_E(cdc_trf_state);
//            }
//            if ((USBHandleBusy(CDCDataInHandle) != 0)) {
//                println_E("CDCDataInHandle State = ");
//                p_int_E(USBHandleBusy(CDCDataInHandle));
//                println_E("CDCDataInHandle = ");
//                p_int_E((int32_t)CDCDataInHandle);
//            }
            usbActive = FALSE;
            //resetUsbSystem();
            return;
        }
        if (USBNotOk) {
            println_E("#*#*USB Not ok");
            usbActive = FALSE;
            resetUsbSystem();
            return;
        }
        usb_Buffer_Update();
    }

}

void flush() {

    float start = getMs();

    waitForTxToBeFree();

    float end = getMs() - start;
    if(end>(USB_TIMEOUT/10)){
        println_W("USB Buffer ready, took:");p_fl_W(end);
    }
    start = getMs();


    if (txSize > 0) {
        putUSBUSART((char *) TxBuffer, txSize);
        //DelayMs(2);
        txSize = 0;
    } else {
        println_W("Zero length packet to send, ignoring");
    }

    waitForTxToBeFree();

    end = getMs() - start;
    if(end>(USB_TIMEOUT/10)){
        println_W("USB Flushed OK, took:");p_fl_W(end);
    }
}

BYTE isUSBActave() {
    return usbActive;
}

void forceOpenUSB() {
    usbActive = TRUE;
}

int USBPutArray(BYTE* stream, int num) {
    if (isUSBActave() == FALSE) {
        //println_I("USB inactive, bailing out");
        return 0;
    }
    //UINT16 i;

    usb_Buffer_Update();
    if (USBNotOk) {
        usbActive = FALSE;
        return 0;
    } else {
        int packetLen = num;
        int packetIndex = 0;
        int i;
        //if(num>(TxPrivateSize)) {
        if (num > TxPrivateSize) {
            println_I("Packet too large for USB buffer");
            while (packetLen > TxPrivateSize) {
                for (i = 0; i < TxPrivateSize; i++) {
                    TxBuffer[i] = stream[packetIndex++];
                    packetLen--;
                }
                println_W("Sending chunk ");
                printStream_I(TxBuffer, i);
                txSize = i;
                flush();
            }
            for (i = 0; i < packetLen; i++) {
                TxBuffer[i] = stream[packetIndex++];
            }
            println_I("Sending chunk ");
            printStream_I(TxBuffer, i);
            txSize = i;
            flush();
        } else {
            //println_I("Packet small enough for USB buffer");
            for (i = 0; i < num; i++) {
                TxBuffer[i] = stream[i];
            }
            //println_I("Sending all ");printStream_I(TxBuffer,num);
            txSize = i;
            flush();
        }
    }
    return TRUE;
}

WORD GetNumUSBBytes(void) {
    usb_Buffer_Update();
    //printBufferState(GetPICUSBFifo());
    //println_I("Update Buffer = ");
    //	BYTE_FIFO_STORAGE* fifo = GetPICUSBFifo();

    WORD data = FifoGetByteCount(GetPICUSBFifo());
    //p_int_I(data);
    return data;
}
static WORD i,gSize;
static BYTE err;
void usb_Buffer_Update(void) {
    if (USBNotOk) {
        usbActive = FALSE;
        return;
    }
    USBMaskInterrupts();
    USBDeviceTasksLocal();
    CDCTxService();
    gSize = getsUSBUSART((char *) RxTmpBuffer, USB_BUFFER_SIZE);
    USBUnmaskInterrupts();
    if (gSize > 0) {
        for (i = 0; i < gSize; i++) {
            do {
                FifoAddByte(GetPICUSBFifo(), RxTmpBuffer[i], & err);
            } while (err != FIFO_OK);
            gotData++;
            usbActive = TRUE;
        }
    }

}

