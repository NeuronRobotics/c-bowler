/*
 * Bowler_Transport.c
 *
 *  Created on: May 27, 2010
 *      Author: hephaestus
 */
#include "Bowler/Bowler.h"
//#define minSize ((BowlerHeaderSize)+4)
#define minSize 1

void allign(BowlerPacket * Packet,BYTE_FIFO_STORAGE * fifo){

	int first = 0;
	do
	{
		FifoReadByteStream(Packet->stream,1,fifo);
		if((Packet->use.head.ProtocolRevision != BOWLER_VERSION)){
			if(first==0){
				println("##Junking bad first byte. Fifo Size=");p_ul(calcByteCount(fifo));print(" [");
			}
			first++;
			print(" 0x");prHEX8(Packet->use.head.ProtocolRevision);
			BYTE b;
			if(getNumBytes(fifo)==0)
				return;
			//StartCritical();
			getStream(& b,1,fifo);
			//EndCritical();

		}
	}while(getNumBytes(fifo)>0 && (Packet->use.head.ProtocolRevision != BOWLER_VERSION));
	if(first>0){
		println("##Junked total:");p_ul(first);
	}
}

BOOL _getBowlerPacket(BowlerPacket * Packet,BYTE_FIFO_STORAGE * fifo, BOOL debug){
	if(debug)
		enableDebug();
	BOOL PacketCheck=FALSE;
	UINT16 PacketLegnth=0;
	if (getNumBytes(fifo) == 0 ) {
		return FALSE;//Not enough bytes to even be a header, try back later
	}

	allign(Packet,fifo);

	if (getNumBytes(fifo) < ((BowlerHeaderSize)+4)) {
		if(debug){
			println("Current num bytes: ");p_ul(getNumBytes(fifo));
		}
		return FALSE;//Not enough bytes to even be a header, try back later
	}
	FifoReadByteStream(Packet->stream,BowlerHeaderSize,fifo);
	PacketCheck=FALSE;
	while(PacketCheck==FALSE){
		if( (Packet->use.head.ProtocolRevision != BOWLER_VERSION)
				|| (CheckCRC(Packet)==FALSE)

		  ){
			if(Packet->use.head.ProtocolRevision != BOWLER_VERSION){
				println("###Bad first byte=");
			}else if(CheckCRC(Packet)==FALSE){
				println("###Bad crc check=");
			}
			prHEX8(Packet->use.head.ProtocolRevision);print(" Fifo Size=");p_ul(calcByteCount(fifo));
			BYTE b;
			if(getNumBytes(fifo)==0)
				return FALSE;
			//StartCritical();
			getStream(& b,1,fifo);//junk out one
			//EndCritical();
			FifoReadByteStream(Packet->stream,BowlerHeaderSize,fifo);
		}else{
			if(debug){
				//println("Got header");
			}
			PacketCheck=TRUE;
		}
		if (getNumBytes(fifo) < minSize) {
			println("##Failed to allign apacket");
			allign(Packet,fifo);
			return FALSE;//Not enough bytes to even be a header, try back later
		}
	}
	PacketLegnth  = Packet->use.head.DataLegnth;
	if(PacketLegnth<4){
		if(debug){
			println("#*#*Warning, packet has no RPC");
		}
	}
	UINT16 totalLen = PacketLegnth+BowlerHeaderSize;
	// See if all the data has arived for this packet
	if (getNumBytes(fifo)>=(totalLen) ){
		if(debug){
			//println("**Found packet, ");p_ul(totalLen);//print(" Bytes, pulling out of buffer");
		}
		//StartCritical();
		getStream(Packet->stream,totalLen,fifo);
		//EndCritical();
		return  TRUE;
	}
	if(debug){
		println("Header ready, but data is not yet. Need: ");p_ul(totalLen);print(" have: ");p_ul(getNumBytes(fifo));
	}
	return FALSE;
}
BOOL GetBowlerPacket(BowlerPacket * Packet,BYTE_FIFO_STORAGE * fifo){
	return _getBowlerPacket(Packet,fifo, FALSE);
}
BOOL GetBowlerPacketDebug(BowlerPacket * Packet,BYTE_FIFO_STORAGE * fifo){
	//enableDebug();
	return _getBowlerPacket( Packet, fifo, TRUE);
}
/**
 * @return returns the number of bytes in the fifo
 */
UINT16 getNumBytes(BYTE_FIFO_STORAGE * fifo){
	return (UINT16)calcByteCount(fifo);
}
/**
 * get a stream of this length from the connection
 */
UINT16 getStream(BYTE *packet,UINT16 size,BYTE_FIFO_STORAGE * fifo){
	return FifoGetByteStream(fifo,packet,size);
}

void FixPacket(BowlerPacket * Packet){
	extern MAC_ADDR MyMAC;
	BYTE i;
	//Ensure the packet going upstream is valid
	for (i=0;i<6;i++){
		Packet->use.head.MAC.v[i]=MyMAC.v[i];
	}
	Packet->use.head.ProtocolRevision=BOWLER_VERSION;
	SetCRC(Packet);
}

BOOL PutBowlerPacket(BowlerPacket * Packet){
	Packet->use.head.ResponseFlag=1;
	FixPacket(Packet);
	return putStream(Packet->stream,GetPacketLegnth(Packet));
}
