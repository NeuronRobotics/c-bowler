/**
 * This is a sample to demonstrate using the Bowler embedded C stack.
 */

#include <stdio.h>
#include "Bowler/Bowler.h"

int main(void){
	/**
	 * User code must implement a few functions needed by the stack internally
	 * This Platform specific code can be stored in the Platform directory if it is universal for all
	 * implementations on the given Platform.
	 * float getMs(void) Returns the current miliseconds since the application started
	 * StartCritical()   Starts a critical protected section of code
 	 * EndCritical()     Ends the critical section and returns to normal opperation
	 */
	printf("\r\nStarting Sample Program\r\n");

	/**
	 * First we are going to put together dummy arrays of packet data. These are examples of a _png send and response.
	 * These would not exist in your implementation but would come in from the physical layer
	 */
	BYTE received[] = 	{0x03, 0x74 , 0xf7 , 0x26 , 0x80 , 0x00 , 0x79 , 0x10 , 0x00 , 0x04 , 0xa1 , 0x5f , 0x70 , 0x6e , 0x67 };
	BYTE response[] = 	{0x03, 0x74 , 0xf7 , 0x26 , 0x80 , 0x00 , 0x79 , 0x20 , 0x80 , 0x04 , 0x31 , 0x5f , 0x70 , 0x6e , 0x67 };

	/**
	 * Now we begin to set up the stack features. The first step is to set up the FIFO to receive the data coming in asynchronusly
	 *
	 */
	BYTE privateRXCom[sizeof(BowlerPacket)];//make the buffer at least big enough to hold one full sized packet
	BYTE_FIFO_STORAGE store;//this is the FIFO data storage struct. All interactions with the circular buffer will go through this.
	/**
	 * Next we initialize the buffer
	 */
	InitByteFifo(&store,// the pointer to the storage struct
			privateRXCom,//pointer the the buffer
			sizeof(privateRXCom));//the size of the buffer

	/**
	 * Now we load the buffer with the the packet that we "Received"
	 * This step would come in from the physical layer, usually on
	 * an interrupt on a mcu.
	 */
	int i=0;
	for (i=0;i<sizeof(received);i++){
		BYTE err;// this is a stack error storage byte. See Bowler/FIFO.h for response codes
		BYTE b= received[i];// This would be a new byte from the physical layer
		FifoAddByte(&store, b, &err);// This helper function adds the byte to the storage buffer and manages the read write pointrs.
	}
	printf("\r\nData loaded into packet\r\n");
	/**
	 * We have now loaded a packet into the storage struct 'store'
	 * All the while we can be polling the storage struct for a new packet
	 */
	BowlerPacket myLocalPacket; // decalre a packet struct to catch the parsed packet from the asynchronus storage buffer
	while(GetBowlerPacket(&myLocalPacket,// pointer to the local packet into which to store the parsed packet
			&store// storage struct from which the packet will be checked and parsed.
			) == FALSE){// Returns true when a packet is found and pulled out of the buffer
		// wait because there is no packet yet
	}

	/**
	 * At this point the packet has been parsed and pulled out of the buffer
	 */
	printf("\r\nGot new Packet:\r\n");
	setPrintLevelNoPrint();// enable the stack specific printer. If you wish to use this feature putCharDebug(char c) needs to be defined
	printPacket(&myLocalPacket, INFO_PRINT);



}
