/*

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
#include "Bowler/Bowler.h"
#include "arch/AVR/BowlerConfig.h"

boolean okToPrint(Print_Level l);
//#if defined(__AVR_ATmega324P__)
	#define comBuffSize (MiniPacketDataSize+4+BowlerHeaderSize)
//#else
//	#define comBuffSize (FullPacketDataSize+4+BowlerHeaderSize)
//#endif
static uint8_t privateRXCom[comBuffSize];
static BYTE_FIFO_STORAGE store;
static uint64_t TimerOFcount=0;
static uint32_t TimerOFcountUpper=0;

boolean GetBowlerPacket_arch(BowlerPacket * Packet){
	return GetBowlerPacket(Packet,&store);
}

/**
 * send the array out the connection
 */
uint16_t putStream(uint8_t *packet,uint16_t size){
	uint16_t i;
	for (i=0;i<size;i++){
		WriteAVRUART0(packet[i]);
	}
	return i;
}
/**
 * get the time in ms
 */
float getMs(void){
	float upper = (((float)TimerOFcountUpper)*((float)4294967294ul))/18.0;
	return (((float)GetTimeTicks())/18.0)+upper;
}
/**
 * send this char to the print terminal
 */
void EnableDebugTerminal(void);
void putCharDebug(char c){
	WriteAVRUART1(c);

	//return true; 
}
/**
 * Start the scheduler
 */
void startScheduler(void){
	TimerOFcount=0;
	TCCR1Abits._WGM =0x00;// Normal , 0xffff top, 0x0000 bottom
	TCCR1Bbits._CS = 5;//  value CLslk I/O/1024 (From prescaler)
	TIMSK1bits._TOIE1=1;
}


void serial_init(unsigned int bittimer);


/**
 * Private helpers
 */
uint64_t GetTimeTicks(void){
	return (uint64_t) (TimerOFcount+TCNT1);
}

ISR(TIMER1_OVF_vect){//timer 1 overflow interrupt
	//StartCritical();
	uint64_t before = TimerOFcount;
	TimerOFcount+=0x10000;
	if(TimerOFcount<before){
		TimerOFcount=0;
		TCNT1=0;
		TimerOFcountUpper++;
	}
	//EndCritical();
}


// Communication HAL

/**
 * Public functions, must be filled
 */
void AVR_Bowler_HAL_Init(void){
	StartCritical();
	serial_init( (( F_CPU /INTERNAL_BAUD / 16 ) - 1));
#if defined(DEBUG)
	EnableDebugTerminal();
#endif
	//print("com:");
	InitByteFifo(&store,privateRXCom,comBuffSize);
	InitFlagPins();
	EndCritical();
}
void WriteAVRUART0(uint8_t val){
	while(FlagAsync  == 0 ); // Wait for controller to be ready
	while ((UCSR0A & (1<<UDRE0)) == 0 );
	UDR0 = val;
	_delay_us(UARTDELAY);
}

void WriteAVRUART1(uint8_t val){
	if (UCSR1B == 0)
		return;
	while ((UCSR1A & (1<<UDRE1)) == 0 );
		UDR1 = val;
	_delay_us(UARTDELAY);
}


/**
 * Private helpers
 */
ISR(USART0_RX_vect){
	StartCritical();
	uint8_t err;
	uint8_t b= UDR0;
	FifoAddByte(&store, b, &err);
	EndCritical();
	//print("Got [0x");prHEX8(b);print("]\n");
}



void serial_init(unsigned int bittimer)
{
	/* Set the baud rate */
	UBRR0H = (unsigned char) (bittimer >> 8);
	UBRR0L = (unsigned char) bittimer;
	//UBRR0H = 0;
	//UBRR0L = 64;

	/* set the framing to 8E1 */
	UCSR0C = (_BV(UCSZ00)|_BV(UCSZ01)| _BV(UPM01));
//	/* set the framing to 8N1 */
//	UCSR0C = (_BV(UCSZ00)|_BV(UCSZ01));

    /* rx interrupts enabled, rx and tx enabled, 8-bit data */
    UCSR0B =( _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0)  ) ;
    UCSR0A = 0x00;

    //EnableDebugTerminal();
}

void EnableDebugTerminal(void){
	unsigned int bittimer;
	bittimer=(( F_CPU / 115200  / 16 ) - 1);
	/* Set the baud rate */
	UBRR1H = (unsigned char) (bittimer >> 8);
	UBRR1L = (unsigned char) bittimer;

	/* set the framing to 8N1 */
	UCSR1C = ((1<< UCSZ10)|(1<< UCSZ11));
	/* tx enabled, 8-bit data */
	UCSR1B =( _BV(TXEN1));
	UCSR1A = 0x00;
}

void showString (PGM_P s,Print_Level l,char newLine) {
	if(!okToprint(l)){
		return;
	}
	if(newLine){
		putCharDebug('\n');
		putCharDebug('\r');
	}
	setColor(l);
	char c;
	while ((c = pgm_read_byte(s++)) != 0)
		putCharDebug(c);
}
