/**
 * @file AdvancedBowlerExample.c
 *
 * Created on: Feb 28, 2013
 * @author hephaestus
 */


#include <stdio.h>
#include "Bowler/Bowler.h"
BYTE test__png(BowlerPacket * Packet);
BYTE test_rtst(BowlerPacket * Packet);

const char testName[] = "bcs.test.*;0.3;;";

static NAMESPACE_LIST bcsTest ={	testName,// The string defining the namespace
									NULL,// the first element in the RPC string
									NULL,// no async for this namespace
									NULL// no initial elements to the other namesapce field.
};

static RPC_LIST bcsTest__png={	BOWLER_GET,
								0,
								test__png,
								NULL
};
static RPC_LIST bcsTest_rtst={	BOWLER_GET,
								0,
								test_rtst,
								NULL
};

BYTE test__png(BowlerPacket * Packet){
	printf("\r\ntest__png callback \r\n");
	Packet->use.head.Method = BOWLER_POST;
	Packet->use.head.RPC = GetRPCValue("Mpng");
	Packet->use.head.DataLegnth = 4;
	return TRUE;
}

BYTE test_rtst(BowlerPacket * Packet){
	printf("\r\ntest_rtst callback \r\n");
	Packet->use.head.DataLegnth=5;
	Packet->use.head.Method = BOWLER_POST;
	Packet->use.data[0]=37;
	return TRUE;
}

NAMESPACE_LIST * getBcsTestNamespace(){
	bcsTest__png.rpc = GetRPCValue("_png");
	bcsTest_rtst.rpc = GetRPCValue("rtst");
	addRpcToNamespace(&bcsTest,& bcsTest__png);
	addRpcToNamespace(&bcsTest,& bcsTest_rtst);
	return &bcsTest;
}


void advancedBowlerExample(){
	/**
	 * User code must implement a few functions needed by the stack internally
	 * This Platform specific code can be stored in the Platform directory if it is universal for all
	 * implementations on the given Platform.
	 * float getMs(void) Returns the current milliseconds since the application started
	 * StartCritical()   Starts a critical protected section of code
 	 * EndCritical()     Ends the critical section and returns to normal operation
	 */
	printf("\r\nStarting Sample Program\r\n");
	int pngtmp = GetRPCValue("_png");
	if(pngtmp != _PNG){
		printf("FAIL Expected %i got %i\r\n",_PNG,pngtmp);
		return;
	}

	/**
	 * First we are going to put together dummy array of packet data. These are examples of a _png receive and a custom response.
	 * These would not exist in your implementation but would come in from the physical layer
	 */
	BowlerPacket myPngPacket;
	myPngPacket.use.head.RPC = GetRPCValue("_png");
	myPngPacket.use.head.MessageID = 1;// Specify namespace 1 for the collision detect
	myPngPacket.use.head.Method = BOWLER_GET;
	myPngPacket.use.head.DataLegnth=4;
	FixPacket(&myPngPacket);// Set up the stack content

	BowlerPacket myNamespacTestPacket;
	myNamespacTestPacket.use.head.RPC = GetRPCValue("rtst");
	myNamespacTestPacket.use.head.Method = BOWLER_GET;
	myNamespacTestPacket.use.data[0] = 37;
	myNamespacTestPacket.use.head.DataLegnth=4+1;
	FixPacket(&myNamespacTestPacket);// Set up the stack content


	/**
	 * Now we begin to set up the stack features. The first step is to set up the FIFO to receive the data coming in asynchronously
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

	Bowler_Init();// Start the Bowler stack

	/**
	 * Now we are going to regester what namespaces we implement with the framework
	 */
	addNamespaceToList(getBcsTestNamespace());

	printf("\r\n# of namespaces declared= %i",getNumberOfNamespaces());
	int i=0;
	for(i=0;i<getNumberOfNamespaces();i++){
		NAMESPACE_LIST * nsPtr = getNamespaceAtIndex(i);
		printf("\r\nNamespace %s at index %i",nsPtr->namespaceString,i);
	}

	/**
	 * Now we load the buffer with the the packet that we "Received"
	 * This step would come in from the physical layer, usually on
	 * an interrupt on a mcu.
	 */

	for (i=0;i<GetPacketLegnth(&myPngPacket);i++){
		BYTE err;// this is a stack error storage byte. See Bowler/FIFO.h for response codes
		BYTE b= myPngPacket.stream[i];// This would be a new byte from the physical layer
		FifoAddByte(&store, b, &err);// This helper function adds the byte to the storage buffer and manages the read write pointers.
	}
	/**
	 * Next we load the new namespace packet
	 */
	i=0;
	for (i=0;i<GetPacketLegnth(&myNamespacTestPacket);i++){
		BYTE err;// this is a stack error storage byte. See Bowler/FIFO.h for response codes
		BYTE b= myNamespacTestPacket.stream[i];// This would be a new byte from the physical layer
		FifoAddByte(&store, b, &err);// This helper function adds the byte to the storage buffer and manages the read write pointers.
	}

	printf("\r\nData loaded into packet\r\n");
	/**
	 * We have now loaded a packet into the storage struct 'store'
	 * All the while we can be polling the storage struct for a new packet
	 */
	BowlerPacket myLocalPacket; // Declare a packet struct to catch the parsed packet from the asynchronous storage buffer
	while(getNumBytes(&store)>0){
		while(Bowler_Server_Static(&myLocalPacket,// pointer to the local packet into which to store the parsed packet
				&store// storage struct from which the packet will be checked and parsed.
				) == FALSE){// Returns true when a packet is found and pulled out of the buffer
			// wait because there is no packet yet
		}

		/**
		 * At this point the packet has been parsed and pulled out of the buffer
		 * The Static server will have also called the call backs to pars the packet, so
		 * the response should be loaded up to send back
		 */
		setPrintLevelInfoPrint();// enable the stack specific printer. If you wish to use this feature putCharDebug(char c) needs to be defined

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
}


