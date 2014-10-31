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



//  uint8_t privateRX[BOWLER_PacketSize];
//  BYTE_FIFO_STORAGE store;
  BYTE_FIFO_STORAGE * my_store;
  uint16_t gotDataUart = false;

boolean GotUARTData(void){
	return gotDataUart>0;
}


BYTE_FIFO_STORAGE * GetPICUARTFifo(){
	return my_store;
}

void SetPICUARTFifo(BYTE_FIFO_STORAGE * s){
	my_store=s;
}
void newByteUartHal(){
	//int timeout =0;
	uint8_t err;
	if(DataRdyUART1())
		FifoAddByte(my_store,UARTGetDataByte(UART1),&err);
	
}

void __ISR(_UART_1_VECTOR, ipl7) U1_ISR(void){
	//StartCritical();
	//int tick =8;
	if (INTGetFlag(INT_SOURCE_UART_RX(UART1))){

		newByteUartHal();
		INTClearFlag(INT_SOURCE_UART_RX(UART1));
//		while(DataRdyUART1() && tick>0){
//			newByteUartHal();
//			tick--;
//		}
	}
	else if ( INTGetFlag(INT_SOURCE_UART_TX(UART1)) ) {

		newByteUartHal();
		INTClearFlag(INT_SOURCE_UART_TX(UART1));
		//println("&@&@&&@&@&@ wtf tx");
	}
	else if(INTGetFlag(INT_SOURCE_UART_ERROR(UART1))){

		newByteUartHal();
		INTClearFlag(INT_SOURCE_UART_ERROR(UART1));
//		while(DataRdyUART1() && tick>0){
//			newByteUartHal();
//			tick--;
//		}
		//println("&@&@&&@&@&@ uart ERROR");
	}
	else if(INTGetFlag(INT_SOURCE_UART(UART1))){

		newByteUartHal();
		INTClearFlag(INT_SOURCE_UART(UART1));
		//println("&@&@&&@&@&@ generic uart");
	}

	//EndCritical();
}

uint32_t CalcBaud(uint32_t Baud){
	uint32_t closest;
	closest = ((GetPeripheralClock()+8ul*Baud)/16/Baud-1);
	return closest;
}
void Pic32UARTSetBaud(int baud){

	UARTEnable(UART1,UART_DISABLE);

	//ConfigUARTOpenCollector();
	ConfigUARTRXTristate();

	UARTSetFifoMode(UART1, UART_INTERRUPT_ON_RX_NOT_EMPTY);
	UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
	UARTSetDataRate(UART1, GetPeripheralClock(), baud );
	UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

//	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART(UART1)		, INT_DISABLED);
	INTEnable(INT_SOURCE_UART_TX(UART1)		, INT_DISABLED);
	INTEnable(INT_SOURCE_UART_ERROR(UART1)	, INT_ENABLED);
	INTEnable(INT_SOURCE_UART_RX(UART1)		, INT_ENABLED);

	INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_7);
	INTSetVectorSubPriority(INT_VECTOR_UART(UART1), INT_SUB_PRIORITY_LEVEL_0);
}
void Pic32UART_HAL_INIT(int BAUDRATE){
	Pic32UARTSetBaud(BAUDRATE);
	//InitByteFifo(&store,privateRX,sizeof(privateRX));
	//my_store=&store;
}

void Pic32UARTGetArray(uint8_t *packet,uint16_t size){
	gotDataUart-=size;
	FifoGetByteStream(my_store,packet,size);
}

uint16_t Pic32Get_UART_Byte_Count(void){
	return FifoGetByteCount(my_store);
}

void Pic32UARTPutArray(uint8_t *packet,uint16_t size){
	uint16_t i;
	for (i=0;i<size;i++){
		WriteUART_COM(packet[i]);
	}
}


