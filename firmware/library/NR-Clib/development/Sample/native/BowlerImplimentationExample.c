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
	setPrintLevelInfoPrint();// enable the stack specific printer. If you wish to use this feature putCharDebug(char c) needs to be defined
	printf("\r\nGot new Packet:\r\n");
	printPacket(&myLocalPacket, INFO_PRINT);

	/**
	 * Here is where you would read the packet data and perform some opperation based on that data
	 */
	if(myLocalPacket.use.head.RPC == _PNG){
		printf("\r\nPacket is a _png packet.. OK!\r\n");
	}

	/**
	 * Now the packet has bee read, we re-use the memory for the response to the host
	 */
	myLocalPacket.use.head.RPC = GetRPCValue("myrp");//set the RPC
	myLocalPacket.use.head.Method = BOWLER_POST;   // set the metond as a status
	INT32_UNION temp;//a 32 bit integer struct to allow for easy breaking up into bytes for transport
	temp.Val=3742;//put some data into the packet
	myLocalPacket.use.data[0]=temp.byte.FB;
	myLocalPacket.use.data[1]=temp.byte.TB;
	myLocalPacket.use.data[2]=temp.byte.SB;
	myLocalPacket.use.data[3]=temp.byte.LB;
	// load a single byte
	myLocalPacket.use.data[4] = 128;
	myLocalPacket.use.head.DataLegnth = 4+ // BYTES in the RPC field
										4+ // BYTES in the 32 bit integer
										1; // The last single byte
	FixPacket(&myLocalPacket);// This is a helper function to validate the packet before sending. It makes sure the MAC address is correct and the CRC is correct

	int packetLength = GetPacketLegnth(&myLocalPacket); // helper function to get packet length

	printf("\r\nPreparing to send:\r\n");
	printPacket(&myLocalPacket, INFO_PRINT);

	printf("\r\nSending Packet Data back out: [ ");
	for(i=0;i< packetLength;i++){
		//This would be sending to the physical layer. For this example we are just printing out the data
		printf(" %i ",myLocalPacket.stream[i]);
	}
	printf(" ] \r\n");


}
