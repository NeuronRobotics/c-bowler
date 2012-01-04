/*
 ============================================================================
 Name        : NRLIB_Test.c
 Author      : Kevin Harrington
 Version     :
 Copyright   : BSD
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Bowler/Bowler.h"

int main(void) {
	println("Bowler Print Test");

	BowlerPacket p;

	BYTE privateRX[BOWLER_PacketSize];
	BYTE_FIFO_STORAGE store;

	InitByteFifo(&store,privateRX,sizeof(privateRX));

	BYTE err;
	int i;
	int j;

	for (j=0;j<200;j++){
		p.use.head.RPC = GetRPCValue("test");
		p.use.head.DataLegnth = 4+2;
		p.use.data[0]=0xff;
		p.use.data[1]=j;
		FixPacket(&p);
		//println("TX>>\n");printPacket(&p);
		for(i=0;i<BowlerHeaderSize+p.use.head.DataLegnth;i++){
			//Load above packet into the fifo
			FifoAddByte(&store,p.stream[i],&err);
		}
		//Run the packet parser against the fifo.
		if(GetBowlerPacket(&p,&store)){
			println("RX<<\n");printPacket(&p);
		}else{
			println("Failed to get packet back\n");
			printFiFoState(&store,p.stream);
			return 1;
		}
	}


	println("Done!");
	return EXIT_SUCCESS;
}
