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
#include "Bowler/Bowler.h"
#if defined(__PIC32MX__)
	extern MAC_ADDR MyMAC __attribute__ ((section (".scs_global_var")));
#else
	extern MAC_ADDR MyMAC;
#endif
static BYTE i;
static UINT32_UNION l;
BYTE CheckCRC(BowlerPacket *Packet){
	UINT16 v=0;
	for (i=0;i<10;i++)
		v+=Packet->stream[i];
	if(Packet->use.head.CRC ==(BYTE)(v & 0x00ff))
		return TRUE;
	return FALSE;
}
void SetCRC(BowlerPacket * Packet){
	UINT16 v=0;
	for (i=0;i<10;i++)
		v+=Packet->stream[i];
	Packet->use.head.CRC = (BYTE) (v & 0x00ff);
}



UINT16 READY(BowlerPacket *Packet,BYTE code,BYTE trace){
	Packet->use.head.Method = BOWLER_STATUS;
	Packet->use.head.RPC = GetRPCValue("_rdy");
	Packet->use.head.MessageID=0;
	Packet->use.head.DataLegnth = 6;
	Packet->use.data[0]=code;
	Packet->use.data[1]=trace;
	return 6;
}
UINT16 ERR(BowlerPacket *Packet,BYTE code,BYTE trace){
	Packet->use.head.Method = BOWLER_STATUS;
	Packet->use.head.RPC = GetRPCValue("_err");
	Packet->use.head.MessageID=0;
	Packet->use.head.DataLegnth = 6;
	Packet->use.data[0]=code;
	Packet->use.data[1]=trace;
	return 6;
}




UINT16 SetPacketLegnth(BowlerPacket *Packet,BYTE len){
	//Packet.stream[DataSizeIndex]=(BYTE)(len&0x00ff);
	Packet->use.head.DataLegnth = len;
	return len;
}
UINT16 GetPacketLegnth(BowlerPacket *Packet){
	return  BowlerHeaderSize + Packet->use.head.DataLegnth ;
}
UINT16 DataLegnthFromPacket(BowlerPacket *Packet){
	return  Packet->use.head.DataLegnth ;
}
void copyPacket(BowlerPacket * from,BowlerPacket * to){
	BYTE i;
	for(i=0;i<BowlerHeaderSize+from->use.head.DataLegnth;i++){
		to->stream[i]=from->stream[i];
	}
}

UINT32 Bytes2Int32(BYTE a,BYTE b,BYTE c,BYTE d){
	return (   (((UINT32)a)<<24) + (((UINT32)b)<<16) + (((UINT32)c)<<8) + ((UINT32)d) );
}
UINT32 GetRPCValue(char * data){
	l.byte.FB = data[3];
	l.byte.TB = data[2];
	l.byte.SB = data[1];
	l.byte.LB = data[0];
	return l.Val;
}

BYTE CheckAddress(BYTE * one,BYTE * two){
	for (i=0;i<6;i++){
		if (one[i] != two[i])
			return FALSE;
	}
	return TRUE;
}

float interpolate(INTERPOLATE_DATA * data, float currentTime){
	float back;
	float diffTime;
	if(data->setTime > 0){
		diffTime = currentTime-data->startTime;
		if((diffTime < data->setTime) && (diffTime>0)){
			float elapsed = 1-((data->setTime-diffTime)/data->setTime);
			float tmp=((float)data->start+(float)(data->set-data->start)*elapsed);

			if(data->set > data->start){
				if((tmp>data->set)||(tmp<data->start))
					tmp=data->set;
			}else{
				if((tmp<data->set) || (tmp>data->start))
					tmp=data->set;
			}
			back=tmp;
		}else{
			// Fixes the overflow case and the timeout case
			data->setTime=0;
			back=data->set;
		}
	}else{
		back=data->set;
		data->setTime = 0;
	}
	return back;
}


